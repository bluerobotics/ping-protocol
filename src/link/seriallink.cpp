#include <QDebug>

#include "seriallink.h"

SerialLink::SerialLink()
{
    qDebug() << "SerialLink in !";
}

SerialLink::~SerialLink()
{
    qDebug() << "SerialLink out !";
}