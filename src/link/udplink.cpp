#include <QDebug>
#include <QLoggingCategory>
#include <QNetworkDatagram>

#include "udplink.h"

Q_LOGGING_CATEGORY(PING_PROTOCOL_UDPLINK, "ping.protocol.udplink")

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

bool UDPLink::setConfiguration(const QStringList& args)
{
    qCDebug(PING_PROTOCOL_UDPLINK) << args;
    if(args.length() != 2) {
        qCDebug(PING_PROTOCOL_UDPLINK) << "Wrong argument E.g: 0.0.0.0:1234";
        qCDebug(PING_PROTOCOL_UDPLINK) << args;
        return false;
    }
    if(args[0].isEmpty() || args[1].isEmpty()) {
        qCDebug(PING_PROTOCOL_UDPLINK) << "Wrong argument E.g: 0.0.0.0:1234";
        qCDebug(PING_PROTOCOL_UDPLINK) << args;
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

UDPLink::~UDPLink() = default;
