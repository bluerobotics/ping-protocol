#pragma once

#include <QObject>

#include "abstractlink.h"

class Link : public QObject
{
    Q_OBJECT
public:
    Link(AbstractLink::LinkType linkType = AbstractLink::LinkType::None, QString name = QString());
    ~Link();

    //This will handle the connection change
    AbstractLink* self() { return _abstractLink; };

private:
    AbstractLink* _abstractLink;
};