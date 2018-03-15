#include <QDebug>

#include "protocol.h"

Protocol::Protocol()
    : _packer(new Packer())
{
    connect(_packer, &Packer::newPackage, this, &Protocol::emitMessages);
    connect(_packer, &Packer::newRawPackage, this, &Protocol::emitRawMessages);
}

Protocol::~Protocol()
{
}

void Protocol::emitMessages(const QVariantList& package)
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
            emit firmwareVersion(QString("%0.%1v").arg(package[8].toInt()).arg(package[9].toInt()));
            break;

        case Message::GeneralMessageID::gen_device_id:
            emit deviceID(package[6].toInt());
            break;

        case Message::GeneralMessageID::gen_new_data:
            emit newData(package[6].toInt());
            break;

        // Echosounder
        case Message::EchosounderMessageID::es_distance_simple:
            emit echosounderDistance(package[6].toInt());
            emit echosounderConfidence(package[7].toInt());
            break;

        case Message::EchosounderMessageID::es_distance:
            emit echosounderDistance(package[6].toInt());
            emit echosounderConfidence(package[7].toInt());
            emit echosounderPulseUs(package[8].toInt());
            emit echosounderPingNumber(package[9].toInt());
            emit echosounderStart(package[10].toInt());
            emit echosounderLength(package[11].toInt());
            emit echosounderGain(package[12].toInt());
            break;

        case Message::EchosounderMessageID::es_profile: {
            emit echosounderDistance(package[6].toInt());
            emit echosounderConfidence(package[7].toInt());
            emit echosounderPulseUs(package[8].toInt());
            emit echosounderPingNumber(package[9].toInt());
            emit echosounderStart(package[10].toInt());
            emit echosounderLength(package[11].toInt());
            emit echosounderGain(package[12].toInt());

            int numberOfPoints = package[13].toInt();
            emit echosounderNumberOfPoints(numberOfPoints);
            QList<double> points;
            points.reserve(numberOfPoints);
            auto tempList = package.mid(14, numberOfPoints);
            for(auto var : tempList) {
                points.append(var.toInt()/255.0f);
            }
            emit echosounderPoints(points);
            break;
        }

        case Message::EchosounderMessageID::es_range:
            emit echosounderStart(package[6].toInt());
            emit echosounderLength(package[7].toInt());
            break;

        case Message::EchosounderMessageID::es_mode:
            emit echosounderAuto(package[6].toInt() == 1);
            break;

        case Message::EchosounderMessageID::es_rate:
            emit echosounderRate(package[6].toInt());
            break;

        case Message::EchosounderMessageID::es_gain:
            emit echosounderGain(package[6].toInt());
            break;

        case Message::EchosounderMessageID::es_pulse:
            emit echosounderPulseUs(package[6].toInt());
            break;


        // MSS
        case Message::MechanicalScanningSonarMessageID::mss_angle_profile: {
            emit mssAngle(package[6].toFloat()/1000.0f);
            emit mssPulseUs(package[7].toInt());
            emit mssRange(package[8].toInt());
            emit mssGain(package[9].toInt());

            int numberOfPoints = package[10].toInt();
            emit mssNumberOfPoints(numberOfPoints);
            QList<int> points;
            points.reserve(numberOfPoints);
            auto tempList = package.mid(11, numberOfPoints);
            for(auto var : tempList) {
                points.append(var.toInt());
            }
            emit mssPoints(points);
            break;
        }
        case Message::MechanicalScanningSonarMessageID::mss_range:
            emit mssRange(package[6].toInt());
            break;

        case Message::MechanicalScanningSonarMessageID::mss_mode:
            emit mssAuto(package[6].toInt() == 1);
            break;

        case Message::MechanicalScanningSonarMessageID::mss_gain:
            emit mssGain(package[6].toInt());
            break;

        case Message::MechanicalScanningSonarMessageID::mss_sector:
            emit msstrainAngle(package[6].toFloat());
            emit msssectorWidth(package[7].toFloat());
            emit mssstepSize(package[8].toInt());
            emit mssLength(package[9].toInt());
            emit msssampleSize(package[10].toInt());
            break;

        default:
            qDebug() << "UNKNOWN MESSAGE TYPE !" << package[4].toInt();
            break;
        }
    }
    emit update();
}

void Protocol::handleData(const QByteArray& data)
{
    _packer->decode(data);
}

void Protocol::request(int messageID)
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

void Protocol::requestGotoBootloader()
{
    QVariantList vars;
    auto byteArray = _packer->createPack(Message::GeneralMessageID::gen_goto_bootloader, vars);
    qDebug() << "~~~Sending to bootloader...";
    emit sendData(byteArray);
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

void Protocol::setEchosounderGain(int gain)
{
    QVariantList vars;
    vars.append(gain);
    auto byteArray = _packer->createPack(Message::EchosounderMessageID::es_gain, vars);
    emit sendData(byteArray);
}

void Protocol::setEchosounderAuto(bool mode)
{
    QVariantList vars;
    vars.append(mode);
    auto byteArray = _packer->createPack(Message::EchosounderMessageID::es_mode, vars);
    emit sendData(byteArray);
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