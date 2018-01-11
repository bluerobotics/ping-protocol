#include <QDebug>

#include "link.h"

#include "filelink.h"
#include "seriallink.h"
#include "tcplink.h"
#include "udplink.h"

Link::Link()
{
    qDebug() << "Link in !";
}

Link::~Link()
{
    qDebug() << "Link out !";
}