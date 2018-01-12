#include <QDebug>

#include "ping.h"

Ping::Ping() :
     _link(new Link(AbstractLink::LinkType::Serial, "Default"))
    ,_protocol(new Protocol())
{
    qDebug() << "Ping in !";
    _link->setConfiguration("/dev/ttyUSB0:115200");
    _link->startConnection();
    if(!_link->abstractLink()->isOpen()) {
        qDebug() << "Connection fail !";
        return;
    }

    connect(_link, &Link::newData, _protocol, &Protocol::handleData);
    connect(_protocol, &Protocol::sendData, _link, &Link::sendData);
}

Ping::~Ping()
{
    _link->disconnect();
    qDebug() << "Ping out !";
}