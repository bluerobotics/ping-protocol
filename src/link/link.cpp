#include <QDebug>

#include "link.h"

#include "filelink.h"
#include "seriallink.h"
#include "tcplink.h"
#include "udplink.h"

Link::Link(AbstractLink::LinkType linkType, QString name)
{
    switch(linkType) {
        case AbstractLink::LinkType::Serial :
            _abstractLink = new SerialLink();
            break;
        case AbstractLink::LinkType::Udp :
            _abstractLink = new UDPLink();
            qDebug() << "udp!!!";
            break;
        default :
            qDebug() << "Link not available!";
            return;
    }

    _abstractLink->setParent(this);
    _abstractLink->setName(name);
}

Link::~Link()
{
}