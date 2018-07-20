#include <QDebug>
#include <QLoggingCategory>

#include "link.h"

#include "filelink.h"
#include "pingsimulationlink.h"
#include "seriallink.h"
#include "simulationlink.h"
#include "tcplink.h"
#include "udplink.h"

Q_LOGGING_CATEGORY(PING_PROTOCOL_LINK, "ping.protocol.link")

Link::Link(AbstractLink::LinkType linkType, QString name, QObject* parent)
    : QObject(parent)
    , _abstractLink(nullptr)
{
    switch(linkType) {
        case AbstractLink::LinkType::File :
            _abstractLink = new FileLink();
            break;
        case AbstractLink::LinkType::Serial :
            _abstractLink = new SerialLink();
            break;
        case AbstractLink::LinkType::Udp :
            _abstractLink = new UDPLink();
            break;
        case AbstractLink::LinkType::PingSimulation :
            _abstractLink = new PingSimulationLink();
            break;
        default :
            qCDebug(PING_PROTOCOL_LINK) << "Link not available!";
            return;
    }

    _abstractLink->setParent(this);
    _abstractLink->setName(name);
}

Link::~Link() = default;
