#include <QDebug>
#include <QTimer>

#include "seriallink.h"

SerialLink::SerialLink()
{
    setType(AbstractLink::LinkType::Serial);

    QObject::connect(this, &QIODevice::readyRead, [=]() {
        emit newData(readAll());
    });

    QObject::connect(this, &AbstractLink::sendData, [=](const QByteArray& data) {
        write(data);
    });
}

bool SerialLink::setConfiguration(const QString& arg)
{
    QStringList args = arg.split(':');
    if(args.length() != 2) {
        qDebug() << "Wrong argument E.g: /dev/ttyUSB0:115200";
        return false;
    }
    setPortName(args[0]);
    setBaudRate(args[1].toInt());

    return true;
}

bool SerialLink::finishConnection()
{
    close();
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