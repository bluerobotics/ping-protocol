#include <QDebug>
#include <QNetworkDatagram>

#include "udplink.h"

UDPLink::UDPLink()
{
    setType(AbstractLink::LinkType::Udp);

    QObject::connect(this, &QIODevice::readyRead, [=]() {
        emit newData(receiveDatagram().data());
    });

    QObject::connect(this, &AbstractLink::sendData, [=](const QByteArray& data) {
        writeDatagram(data, _hostAddress, _port);
    });
}

bool UDPLink::setConfiguration(const QString& arg)
{
    QStringList args = arg.split(':');
    qDebug() << args;
    if(args.length() != 2) {
        qDebug() << "Wrong argument E.g: 0.0.0.0:1234";
        qDebug() << arg;
        return false;
    }
    if(args[0].isEmpty() || args[1].isEmpty()) {
        qDebug() << "Wrong argument E.g: 0.0.0.0:1234";
        qDebug() << arg;
        return false;
    }
    _hostAddress = QHostAddress(args[0]);
    _port = args[1].toInt();

    return true;
}

bool UDPLink::finishConnection()
{
    close();
    return true;
}

UDPLink::~UDPLink()
{
}
