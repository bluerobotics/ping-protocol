#include <QDebug>
#include <QTimer>

#include "seriallink.h"

SerialLink::SerialLink()
{
    setType(AbstractLink::LinkType::Serial);
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

bool SerialLink::startConnection()
{
    if(open(QIODevice::ReadWrite)) {
        QObject::connect(this, &QIODevice::readyRead, [=]() {
            emit newData(readAll());
        });

        QObject::connect(this, &AbstractLink::sendData, [=](const QByteArray& data) {
            write(data);
        });
        return true;
    }
    qDebug() << __FUNCTION__ << "Check your connection !";
    return false;
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