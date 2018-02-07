#include <functional>

#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QUrl>

#include "filelink.h"

FileLink::FileLink():
     _openModeFlag(QIODevice::ReadWrite)
    ,_time(QTime::currentTime())
{
    setType(AbstractLink::LinkType::File);

    // This save the data as a structure to deal with the timestamp
    QObject::connect(this, &AbstractLink::sendData, [=](const QByteArray& data) {
        if(_openModeFlag == QIODevice::WriteOnly) {
            static QDataStream out(this);
            QString time = _time.currentTime().toString(_timeFormat);
            Pack pack{time, data};
            out << pack.time << pack.data;
        }
    });
}

bool FileLink::setConfiguration(const QString& arg)
{
    QStringList args = arg.split(':');
    qDebug() << args;
    if(args.length() != 2) {
        qDebug() << "Wrong argument E.g: path/file:format";
        qDebug() << arg;
        return false;
    }
    if(args[0].isEmpty() || args[1].isEmpty()) {
        qDebug() << "Wrong argument E.g: path/file:format";
        qDebug() << arg;
        return false;
    }

    // Read or create the log ?
    // This flag does not change how the file will be open (ReadWrite)
    _openModeFlag = args[1][0] == "r" ? QIODevice::ReadOnly : QIODevice::WriteOnly;

    setFileName(args[0]);

    return true;
}

bool FileLink::startConnection() {
    bool ok = open(QIODevice::ReadWrite);
    // FileLink created as read ?
    if(ok && _openModeFlag == QIODevice::ReadOnly) {
        // Prepare lambda timer !
        static std::function<void(void)> sendNewPackage = [&] {
            // Start lambda vars
            // TODO: `in` need to be outside to update if new log is selected
            static QDataStream in(this);
            static Pack pack;
            static int mSecs(0);

            in >> pack.time >> pack.data;

            // Check if we have a new package
            if(pack.time.isEmpty()) {
                qDebug() << "No more packages !";
                return;
            }
            emit newData(pack.data);
            // Emit package or wait for the correct time
            if(mSecs == 0) {
                // First time, no diffMSecs, run again
                mSecs = QTime::fromString(pack.time, _timeFormat).msecsSinceStartOfDay();
                sendNewPackage();
            } else {
                // Get diff between the two messages, and trigger the next emit
                int diffMSecs = QTime::fromString(pack.time, _timeFormat).msecsSinceStartOfDay() - mSecs;
                // Update mSecs
                mSecs += diffMSecs;
                QTimer::singleShot(diffMSecs, [&]{sendNewPackage();});
            }
        };
        // Start replay
        sendNewPackage();
    }
    return ok;
};

bool FileLink::finishConnection()
{
    close();
    return true;
}

FileLink::~FileLink()
{
}
