#include <QDebug>
#include <QTimer>

#include "protocol.h"

Protocol::Protocol()
{
    qDebug() << "Protocol in !";

    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [=]() {
        requestVersion();
    });
    timer->start(1000);
}

Protocol::~Protocol()
{
    qDebug() << "Protocol out !";
}

void Protocol::handleData(const QByteArray& data)
{
    qDebug() << data;
}

void Protocol::requestVersion()
{
    auto byteArray = _packer.request(Message::GeneralMessageID::gen_get_version);
    emit sendData(byteArray);
}