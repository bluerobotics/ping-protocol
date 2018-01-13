#include <QDebug>
#include <QTimer>

#include "protocol.h"

Protocol::Protocol()
    : _packer(new Packer())
{
    qDebug() << "Protocol in !";

    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [=]() {
        requestVersion();
    });
    timer->start(1000);

    connect(_packer, &Packer::newPackage, [=](QVariantList package){
        qDebug() << "----------------------";
        qDebug() << __FUNCTION__ << package;
        qDebug() << "----------------------";
    });
}

Protocol::~Protocol()
{
    qDebug() << "Protocol out !";
}

void Protocol::handleData(const QByteArray& data)
{
    qDebug() << "Receive:" <<  data;
    qDebug() << _packer->decode(data);
}

void Protocol::requestVersion()
{
    auto byteArray = _packer->request(Message::GeneralMessageID::gen_get_version);
    emit sendData(byteArray);
}