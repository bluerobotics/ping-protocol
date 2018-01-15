#include <QDebug>

#include "abstractlink.h"

AbstractLink::AbstractLink() :
    _type(LinkType::None)
{
}

const AbstractLink& AbstractLink::operator=(const AbstractLink& other)
{
    _autoConnect = other._autoConnect;
    _type = other._type;
    _name = other._name;
    return *this;
}

AbstractLink::~AbstractLink()
{
}