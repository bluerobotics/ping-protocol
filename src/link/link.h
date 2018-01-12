#pragma once

#include <QObject>

#include "abstractlink.h"

class Link : public QObject
{
    Q_OBJECT
public:
    Link(AbstractLink::LinkType linkType = AbstractLink::LinkType::None, QString name = QString());
    ~Link();

    void setConfiguration(const QString& string);
    void connect();

private:
    AbstractLink* _abstractLink;
};