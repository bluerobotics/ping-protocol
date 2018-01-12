#include <QDebug>

#include "ping.h"

Ping::Ping() :
     _link(new Link(AbstractLink::LinkType::Serial, "Default"))
    ,_protocol(new Protocol())
{
    qDebug() << "Ping in !";
    _link->self()->setConfiguration("/dev/ttyUSB0:115200");
    _link->self()->startConnection();
    if(!_link->self()->isOpen()) {
        qDebug() << "Connection fail !";
        return;
    }

    connect(_link->self(), &AbstractLink::newData, _protocol, &Protocol::handleData);
    connect(_protocol, &Protocol::sendData, _link->self(), &AbstractLink::sendData);
}

Ping::~Ping()
{
    _link->disconnect();
    qDebug() << "Ping out !";
}