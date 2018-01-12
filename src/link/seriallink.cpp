#include <QDebug>
#include <QTimer>

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
    if(open(QIODevice::ReadWrite)) {
        QObject::connect(this, &QIODevice::readyRead, [=]() {
            qDebug() << __FUNCTION__ << "new data";
            emit newData(readAll());
        });

        QTimer *timer = new QTimer();
        QObject::connect(timer, &QTimer::timeout, [=]() {
            QByteArray s;
            s.append('B');
            s.append('R');
            s.append((uint8_t)2);
            s.append((uint8_t)0);
            s.append((uint8_t)120);
            s.append((uint8_t)0);
            s.append((uint8_t)0);
            s.append((uint8_t)0);
            s.append((uint8_t)101);
            s.append((uint8_t)0);
            uint16_t checksum = 0;
            for (int i = 0; i < s.length(); i++)
                checksum += (uint8_t) s[i];
            s.append((const char*) &checksum, 2);
            qDebug() << "sending.." << s;
            write(s);
        });
        timer->start(1000);
        return true;
    }
    return false;
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