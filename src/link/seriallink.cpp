#include <QDebug>
#include <QTimer>

#include "seriallink.h"

SerialLink::SerialLink()
{
    setType(AbstractLink::LinkType::Serial);
    qDebug() << "SerialLink in !";
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
            qDebug() << __FUNCTION__ << "new data";
            emit newData(readAll());
        });

        QObject::connect(this, &AbstractLink::sendData, [=](const QByteArray& data) {
            qDebug() << __FUNCTION__ << "sending..." << data;
            write(data);
        });
        return true;
    }
    return false;
}

bool SerialLink::finishConnection()
{
    close();
    return true;
}

SerialLink::~SerialLink()
{
    qDebug() << "SerialLink out !";
}