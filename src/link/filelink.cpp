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
    setType(LinkType::File);

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

bool FileLink::setConfiguration(const LinkConfiguration& linkConfiguration)
{
    _linkConfiguration = linkConfiguration;
    qCDebug(PING_PROTOCOL_FILELINK) << linkConfiguration.toString();
    if(!linkConfiguration.isValid()) {
        qCDebug(PING_PROTOCOL_FILELINK) << LinkConfiguration::errorToString(linkConfiguration.error());
        return false;
    }

    setName(linkConfiguration.name());

    // Read or create the log ?
    // This flag does not change how the file will be open (ReadWrite)
    _openModeFlag = linkConfiguration.args()->at(1)[0] == "r" ? QIODevice::ReadOnly : QIODevice::WriteOnly;

    _file.setFileName(linkConfiguration.args()->at(0));

    return true;
}

bool FileLink::startConnection() {
    bool ok = _file.open(QIODevice::ReadWrite);
    // FileLink created as read ?
    if(ok && _openModeFlag == QIODevice::ReadOnly) {
        Pack pack;
        if(_logThread) {
            // Disconnect LogThread
            disconnect(_logThread, &LogThread::newPackage, this, &FileLink::newData);
            disconnect(_logThread, &LogThread::packageIndexChanged, this, &FileLink::packageIndexChanged);
            disconnect(_logThread, &LogThread::packageIndexChanged, this, &FileLink::elapsedTimeChanged);

            delete _logThread;
        }
        _logThread = new LogThread();
        connect(_logThread, &LogThread::newPackage, this, &FileLink::newData);
        connect(_logThread, &LogThread::packageIndexChanged, this, &FileLink::packageIndexChanged);
        connect(_logThread, &LogThread::packageIndexChanged, this, &FileLink::elapsedTimeChanged);
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
