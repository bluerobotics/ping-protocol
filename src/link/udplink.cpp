#include <QDebug>
#include <QLoggingCategory>
#include <QNetworkDatagram>

#include "udplink.h"

Q_LOGGING_CATEGORY(PING_PROTOCOL_UDPLINK, "ping.protocol.udplink")

UDPLink::UDPLink(QObject* parent)
    : AbstractLink(parent)
    , _udpSocket(new QUdpSocket(parent))
{
    setType(AbstractLink::LinkType::Udp);

    connect(_udpSocket, &QIODevice::readyRead, this, [this]() {
        emit newData(_udpSocket->receiveDatagram().data());
    });

    connect(this, &AbstractLink::sendData, this, [this](const QByteArray& data) {
        _udpSocket->writeDatagram(data, _hostAddress, _port);
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
    _udpSocket->close();
    return true;
}

UDPLink::~UDPLink() = default;
