#include <QDebug>

#include "protocol.h"

Protocol::Protocol()
    : _packer(new Packer())
{
    connect(_packer, &Packer::newPackage, this, &Protocol::emitMessages);
}

Protocol::~Protocol()
{
}

void Protocol::emitMessages(QVariantList package)
{
    switch(package[3].toInt()) {
        // General
        case Message::GeneralMessageID::gen_get_version: {
            // All this strings need to be in message
            if(package[6].toInt() == 1) {
                emit deviceType("Echosounder");
            } else if(package[6].toInt() == 2) {
                emit deviceType("MSS");
            } else {
                emit deviceType("Unknown");
            }

            emit deviceModel(package[7].toInt() == 1 ? "Ping" : "Unknown");
            emit firmwareVersion(QString("v%1.%2").arg(package[8].toInt(), package[9].toInt()));
            break;

        case Message::GeneralMessageID::gen_device_id:
            emit deviceID(package[6].toInt());
            break;

        case Message::GeneralMessageID::gen_new_data:
            emit newData(package[6].toInt());
            break;

        // Echosounder
        case Message::EchosounderMessageID::es_distance_simple:
            emit EchosounderDistance(package[6].toInt());
            emit EchosounderConfidence(package[7].toFloat());
            break;

        case Message::EchosounderMessageID::es_distance:
            emit EchosounderDistance(package[6].toInt());
            emit EchosounderConfidence(package[7].toFloat());
            emit EchosounderPulseUs(package[8].toInt());
            emit EchosounderPingNumber(package[9].toInt());
            emit EchosounderStart(package[10].toInt());
            emit EchosounderLength(package[11].toInt());
            emit EchosounderGain(package[12].toInt());
            break;

        case Message::EchosounderMessageID::es_profile: {
            emit EchosounderDistance(package[6].toInt());
            emit EchosounderConfidence(package[7].toFloat());
            emit EchosounderPulseUs(package[8].toInt());
            emit EchosounderPingNumber(package[9].toInt());
            emit EchosounderStart(package[10].toInt());
            emit EchosounderLength(package[11].toInt());
            emit EchosounderGain(package[12].toInt());

            int numberOfPoints = package[13].toInt();
            emit EchosounderNumberOfPoints(numberOfPoints);
            QList<int> points;
            points.reserve(numberOfPoints);
            auto tempList = package.mid(14, numberOfPoints);
            for(auto var : tempList) {
                points.append(var.toInt());
            }
            emit EchosounderPoints(points);
            break;
        }

        case Message::EchosounderMessageID::es_range:
            emit EchosounderStart(package[6].toInt());
            emit EchosounderLength(package[7].toInt());
            break;

        case Message::EchosounderMessageID::es_mode:
            emit EchosounderAuto(package[6].toInt() == 1);
            break;

        case Message::EchosounderMessageID::es_rate:
            emit EchosounderRate(package[6].toInt());
            break;

        case Message::EchosounderMessageID::es_gain:
            emit EchosounderGain(package[6].toInt());
            break;

        case Message::EchosounderMessageID::es_pulse:
            emit EchosounderPulseUs(package[6].toInt());
            break;


        // MSS
        case Message::MechanicalScanningSonarMessageID::mss_angle_profile: {
            emit MSSangle(package[6].toFloat()/1000.0f);
            emit MSSPulseUs(package[7].toInt());
            emit MSSRange(package[8].toInt());
            emit MSSGain(package[9].toInt());

            int numberOfPoints = package[10].toInt();
            emit MSSNumberOfPoints(numberOfPoints);
            QList<int> points;
            points.reserve(numberOfPoints);
            auto tempList = package.mid(11, numberOfPoints);
            for(auto var : tempList) {
                points.append(var.toInt());
            }
            emit MSSPoints(points);
            break;
        }
        case Message::MechanicalScanningSonarMessageID::mss_range:
            emit MSSRange(package[6].toInt());
            break;

        case Message::MechanicalScanningSonarMessageID::mss_mode:
            emit MSSAuto(package[6].toInt() == 1);
            break;

        case Message::MechanicalScanningSonarMessageID::mss_gain:
            emit MSSGain(package[6].toInt());
            break;

        case Message::MechanicalScanningSonarMessageID::mss_sector:
            emit MSStrainAngle(package[6].toFloat());
            emit MSSsectorWidth(package[7].toFloat());
            emit MSSstepSize(package[8].toInt());
            emit MSSLength(package[9].toInt());
            emit MSSsampleSize(package[10].toInt());
            break;

        default:
            qDebug() << "UNKNOWN MESSAGE TYPE !" << package[4].toInt();
            break;
        }
    }
}

void Protocol::handleData(const QByteArray& data)
{
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