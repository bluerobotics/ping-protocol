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
    _packer->decode(data);
}

void Protocol::request(QVariant messageID)
{
    auto byteArray = _packer->request(messageID);
    emit sendData(byteArray);
}

// General requests
void Protocol::requestVersion()
{
    request(Message::GeneralMessageID::gen_get_version);
}

void Protocol::requestDeviceID()
{
    request(Message::GeneralMessageID::gen_device_id);
}

void Protocol::requestNewData()
{
    request(Message::GeneralMessageID::gen_new_data);
}

// Echosounder requests
void Protocol::requestEchosounderDistanceSimple()
{
    request(Message::EchosounderMessageID::es_distance_simple);
}

void Protocol::requestEchosounderDistance()
{
    request(Message::EchosounderMessageID::es_distance);
}

void Protocol::requestEchosounderProfile()
{
    request(Message::EchosounderMessageID::es_profile);
}

void Protocol::requestEchosounderRange()
{
    request(Message::EchosounderMessageID::es_range);
}

void Protocol::requestEchosounderMode()
{
    request(Message::EchosounderMessageID::es_mode);
}

void Protocol::requestEchosounderRate()
{
    request(Message::EchosounderMessageID::es_rate);
}

void Protocol::requestEchosounderGain()
{
    request(Message::EchosounderMessageID::es_gain);
}

void Protocol::requestEchosounderPulse()
{
    request(Message::EchosounderMessageID::es_pulse);
}

// Mechanical Scanning Sonar requests
void Protocol::requestMSSAngleProfilea()
{
    request(Message::MechanicalScanningSonarMessageID::mss_angle_profile);
}

void Protocol::requestMSSRange()
{
    request(Message::MechanicalScanningSonarMessageID::mss_range);
}

void Protocol::requestMSSMode()
{
    request(Message::MechanicalScanningSonarMessageID::mss_mode);
}

void Protocol::requestMSSGain()
{
    request(Message::MechanicalScanningSonarMessageID::mss_gain);
}