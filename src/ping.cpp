#include <QDebug>

#include "ping.h"

Ping::Ping() :
     _link(new Link(AbstractLink::LinkType::Serial, "Default"))
    ,_protocol(new Protocol())
{
    emit linkUpdate();
    auto serialPorts = _link->self()->listAvailableConnections();
    if(!serialPorts.isEmpty()) {
        auto port = serialPorts[0];
        connectLink(port + ":115200");
    }
}

void Ping::connectLink(const QString& connString)
{
    if(_link->self()->isOpen()) {
        _link->self()->finishConnection();
        disconnect(_link->self(), 0, _protocol, 0);
        disconnect(_protocol, 0, _link->self(), 0);
        disconnect(_protocol, 0, this, 0);
    }

    _link->self()->setConfiguration(connString);
    _link->self()->startConnection();

    if(!_link->self()->isOpen()) {
        qDebug() << "Connection fail !" << connString;
        return;
    }

    qDebug() << "Connection OK !";
    connect(_link->self(), &AbstractLink::newData, _protocol, &Protocol::handleData);
    connect(_protocol, &Protocol::sendData, _link->self(), &AbstractLink::sendData);
    connect(_protocol, &Protocol::update, this, &Ping::protocolUpdate);
}

Ping::~Ping()
{
    _link->self()->finishConnection();
}