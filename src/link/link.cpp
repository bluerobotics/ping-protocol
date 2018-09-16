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

Link::Link(LinkType linkType, QString name, QObject* parent)
    : QObject(parent)
    , _abstractLink(nullptr)
{
    switch(linkType) {
        case LinkType::File :
            _abstractLink = new FileLink();
            break;
        case LinkType::Serial :
            _abstractLink = new SerialLink();
            break;
        case LinkType::Udp :
            _abstractLink = new UDPLink();
            break;
        case LinkType::PingSimulation :
            _abstractLink = new PingSimulationLink();
            break;
        default :
            qCDebug(PING_PROTOCOL_LINK) << "Link not available!";
            return;
    }

    _abstractLink->setParent(this);
    _abstractLink->setName(name);
}

Link::Link(const LinkConfiguration& linkConfiguration, QObject* parent)
    : QObject(parent)
    , _abstractLink(nullptr)
{
    switch(linkConfiguration.type()) {
        case LinkType::File :
            _abstractLink = new FileLink();
            break;
        case LinkType::Serial :
            _abstractLink = new SerialLink();
            break;
        case LinkType::Udp :
            _abstractLink = new UDPLink();
            break;
        case LinkType::PingSimulation :
            _abstractLink = new PingSimulationLink();
            break;
        default :
            qCDebug(PING_PROTOCOL_LINK) << "Link not available!";
            return;
    }

    _abstractLink->setParent(this);
    _abstractLink->setConfiguration(linkConfiguration);
}

Link::~Link() = default;
