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
    ,_logThread(nullptr)
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
        QDataStream in(this);
        Pack pack;
        if(_logThread) {
            // Disconnect LogThread
            QObject::disconnect(_logThread, &LogThread::newPackage, this, &FileLink::newData);
            QObject::disconnect(_logThread, &LogThread::packageIndexChanged, this, &FileLink::packageIndexChanged);
            QObject::disconnect(_logThread, &LogThread::packageIndexChanged, this, &FileLink::elapsedTimeChanged);

            delete _logThread;
        }
        _logThread = new LogThread();
        QObject::connect(_logThread, &LogThread::newPackage, this, &FileLink::newData);
        QObject::connect(_logThread, &LogThread::packageIndexChanged, this, &FileLink::packageIndexChanged);
        QObject::connect(_logThread, &LogThread::packageIndexChanged, this, &FileLink::elapsedTimeChanged);
        while(true) {
            // Get data
            in >> pack.time >> pack.data;

            // Check if we have a new package
            if(pack.time.isEmpty()) {
                qDebug() << "No more packages !";
                break;
            }

            QTime time = QTime::fromString(pack.time, _timeFormat);
            _logThread->append(time, pack.data);
        }
        _logThread->start();
        emit elapsedTimeChanged();
        emit totalTimeChanged();
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
