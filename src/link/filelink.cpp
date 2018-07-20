#include <functional>

#include <QDebug>
#include <QDir>
#include <QLoggingCategory>
#include <QTimer>
#include <QUrl>

#include "filelink.h"

Q_LOGGING_CATEGORY(PING_PROTOCOL_FILELINK, "ping.protocol.filelink")

FileLink::FileLink(QObject* parent)
    : AbstractLink(parent)
    , _openModeFlag(QIODevice::ReadWrite)
    , _time(QTime::currentTime())
    , _inout(&_file)
    , _logThread(nullptr)
{
    setType(AbstractLink::LinkType::File);

    connect(this, &AbstractLink::sendData, this, &FileLink::_writeData);
}

void FileLink::_writeData(const QByteArray& data)
{
    if (!_file.isOpen()) {
        return;
    }

    // This save the data as a structure to deal with the timestamp
    if(_openModeFlag == QIODevice::WriteOnly && _file.isWritable()) {
        QString time = _time.currentTime().toString(_timeFormat);
        Pack pack{time, data};
        _inout << pack.time << pack.data;
    }
}

bool FileLink::setConfiguration(const QStringList& args)
{
    qCDebug(PING_PROTOCOL_FILELINK) << args;
    if(args.length() != 2) {
        qCDebug(PING_PROTOCOL_FILELINK) << "Wrong argument E.g: path/file:format";
        qCDebug(PING_PROTOCOL_FILELINK) << args;
        return false;
    }
    if(args[0].isEmpty() || args[1].isEmpty()) {
        qCDebug(PING_PROTOCOL_FILELINK) << "Wrong argument E.g: path/file:format";
        qCDebug(PING_PROTOCOL_FILELINK) << args;
        return false;
    }

    // Read or create the log ?
    // This flag does not change how the file will be open (ReadWrite)
    _openModeFlag = args[1][0] == "r" ? QIODevice::ReadOnly : QIODevice::WriteOnly;

    _file.setFileName(args[0]);

    return true;
}

bool FileLink::startConnection() {
    bool ok = _file.open(QIODevice::ReadWrite);
    // FileLink created as read ?
    if(ok && _openModeFlag == QIODevice::ReadOnly) {
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
            _inout >> pack.time >> pack.data;

            // Check if we have a new package
            if(pack.time.isEmpty()) {
                qCDebug(PING_PROTOCOL_FILELINK) << "No more packages !";
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
    _file.close();
    return true;
}

FileLink::~FileLink()
{
    delete _logThread;
}
