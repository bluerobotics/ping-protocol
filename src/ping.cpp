#include <QDebug>

#include "ping.h"

Ping::Ping() :
    _link(AbstractLink::LinkType::Serial, "Default")
{
    qDebug() << "Ping in !";
    _link.setConfiguration("/dev/ttyUSB0:115200");
    _link.connect();
    _link.disconnect();
}

Ping::~Ping()
{
    qDebug() << "Ping out !";
}