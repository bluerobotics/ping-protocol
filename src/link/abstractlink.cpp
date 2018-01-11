#include <QDebug>

#include "abstractlink.h"

AbstractLink::AbstractLink()
{
    qDebug() << "AbstractLink in !";
}

AbstractLink::~AbstractLink()
{
    qDebug() << "AbstractLink out !";
}