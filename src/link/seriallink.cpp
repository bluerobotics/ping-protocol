#include <QDebug>

#include "seriallink.h"

SerialLink::SerialLink()
{
    setType(AbstractLink::LinkType::Serial);
    qDebug() << "SerialLink in !";
}

bool SerialLink::setConfiguration(const QStringList& args)
{
    if(args.length() != 2) {
        qDebug() << "Wrong argument E.g: /dev/ttyUSB0:115200";
        return false;
    }
    setPortName(args[0]);
    setBaudRate(args[1].toInt());
    return true;
}

bool SerialLink::connect()
{
    return open(QIODevice::ReadWrite);
}

bool SerialLink::disconnect()
{
    close();
    return true;
}

SerialLink::~SerialLink()
{
    qDebug() << "SerialLink out !";
}