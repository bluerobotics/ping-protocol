#pragma once

#include <QUdpSocket>

#include "abstractlink.h"

class UDPLink : public AbstractLink, QUdpSocket
{
public:
    UDPLink();
    ~UDPLink();

    bool isOpen() final { return isWritable() && isReadable(); };
    bool setConfiguration(const QString& arg) final;
    bool startConnection() final { return open(QIODevice::ReadWrite); };
    bool finishConnection() final;
    QString errorString() final { return QUdpSocket::errorString(); };

private:
    QHostAddress _hostAddress;
    uint _port;
};