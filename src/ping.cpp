#include <QDebug>

#include "ping.h"

Ping::Ping()
{
    qDebug() << "Ping in !";
}

Ping::~Ping()
{
    qDebug() << "Ping out !";
}