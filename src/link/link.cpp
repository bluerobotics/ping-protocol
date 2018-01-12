#include <QDebug>

#include "link.h"

#include "filelink.h"
#include "seriallink.h"
#include "tcplink.h"
#include "udplink.h"

Link::Link(AbstractLink::LinkType linkType, QString name)
{
    qDebug() << "Link in !" << linkType;

    switch(linkType) {
        case AbstractLink::LinkType::Serial :
            _abstractLink = new SerialLink();
            break;
        default :
            qDebug() << "Link not available!";
            return;
    }

    _abstractLink->setParent(this);
    _abstractLink->setName(name);
}

void Link::setConfiguration(const QString& string)
{
    if(string.isEmpty()) {
        qDebug() << "No argument!";
        return;
    }

    QStringList args = string.split(':');
    if(!_abstractLink->setConfiguration(args)) {
        qDebug() << "Configuration not ok... check argument";
    }
}

void Link::startConnection()
{
    if(!_abstractLink->startConnection()) {
        qDebug() << "Mo connection.. Check your arguments or connection";
    }
    connect(_abstractLink, &AbstractLink::newData, this, &Link::newData);
    connect( this, &Link::sendData, _abstractLink, &AbstractLink::sendData);
}

Link::~Link()
{
    qDebug() << "Link out !";
}