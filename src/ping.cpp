#include <QDebug>

#include "ping.h"

Ping::Ping() :
     _link(new Link(AbstractLink::LinkType::Serial, "Default"))
    ,_protocol(new Protocol())
{
    emit linkUpdate();

    connectLink("2:/dev/ttyUSB0:115200");
}

void Ping::connectLink(const QString& connString)
{
    if(_link->self()->isOpen()) {
        _link->self()->finishConnection();
        disconnect(_link->self(), 0, _protocol, 0);
        disconnect(_protocol, 0, _link->self(), 0);
        disconnect(_protocol, 0, this, 0);
    }

    QStringList confList = connString.split(':');
    if(confList.length() != 3) {
        qDebug() << "wrong size !";
        return;
    }
    if(confList[0].toInt() <= 0 || confList[0].toInt() > 5) {
        qDebug() << "wrong arg !";
        return;
    }
    if(_link) {
        delete _link;
    }
    _link = new Link((AbstractLink::LinkType)confList[0].toInt(), "Default");
    confList.removeFirst();
    QString conf = confList.join(':');

    _link->self()->setConfiguration(conf);
    _link->self()->startConnection();
    emit linkUpdate();

    if(!_link->self()->isOpen()) {
        qDebug() << "Connection fail !" << connString;
        emit connectionClose();
        return;
    }

    qDebug() << "Connection OK !";
    connect(_link->self(), &AbstractLink::newData, _protocol, &Protocol::handleData);
    connect(_protocol, &Protocol::sendData, _link->self(), &AbstractLink::sendData);
    connect(_protocol, &Protocol::update, this, &Ping::protocolUpdate);
    emit connectionOpen();
}

Ping::~Ping()
{
    _link->self()->finishConnection();
}
