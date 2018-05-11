#include <QDebug>
#include <QLoggingCategory>
#include <QTimer>

#include "seriallink.h"

Q_LOGGING_CATEGORY(PING_PROTOCOL_SERIALLINK, "ping.protocol.seriallink")

SerialLink::SerialLink()
{
    setType(AbstractLink::LinkType::Serial);

    QObject::connect(this, &QIODevice::readyRead, [=]() {
        emit newData(readAll());
    });

    QObject::connect(this, &AbstractLink::sendData, [=](const QByteArray& data) {
        write(data);
    });

    QObject::connect(this, &QSerialPort::errorOccurred, [=](QSerialPort::SerialPortError error) {
        qCWarning(PING_PROTOCOL_SERIALLINK) << "Error:" << error;
        switch(error) {
            case QSerialPort::DeviceNotFoundError...QSerialPort::UnknownError:
                qCWarning(PING_PROTOCOL_SERIALLINK) << "Error is critical ! Port need to be closed.";
                finishConnection();
                break;
            default:
                qCDebug(PING_PROTOCOL_SERIALLINK) << "Error appear to be not critical. Nothing will be done about it.";
                break;
        }
    });
}

bool SerialLink::setConfiguration(const QString& arg)
{
    QStringList args = arg.split(':');
    if(args.length() != 2) {
        qCDebug(PING_PROTOCOL_SERIALLINK) << "Wrong argument E.g: /dev/ttyUSB0:115200";
        qCDebug(PING_PROTOCOL_SERIALLINK) << arg;
        return false;
    }
    if(args[0].isEmpty() || args[1].isEmpty()) {
        qCDebug(PING_PROTOCOL_SERIALLINK) << "Wrong argument E.g: /dev/ttyUSB0:115200";
        qCDebug(PING_PROTOCOL_SERIALLINK) << arg;
        return false;
    }
    setPortName(args[0]);
    setBaudRate(args[1].toInt());

    return true;
}

bool SerialLink::finishConnection()
{
    close();
    qCDebug(PING_PROTOCOL_SERIALLINK) << "port closed";
    return true;
}

QStringList SerialLink::listAvailableConnections()
{
    static QStringList list;
    auto oldList = list;
    list.clear();
    auto ports = availablePorts();
    for(const auto& port : ports) {
        list.append(port.portName());
    }
    if(oldList != list) {
        emit availableConnectionsChanged();
    }
    return list;
}

SerialLink::~SerialLink()
{
}
