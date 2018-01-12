#include <QDebug>

#include "message.h"

Message::Message()
{
    qDebug() << "Message in !";
    qDebug() << string(GeneralMessageID::gen_goto_bootloader);
    qDebug() << packString(EchosounderMessageID::es_profile);
}

Message::~Message()
{
    qDebug() << "Message out !";
}

const QByteArray Message::checksumPackString()
{
    return "<H";
}

const QByteArray Message::headerPackString()
{
    /**
     * 0 'B'
     * 1 'R'
     * 2-3 length
     * 4-5 msg_id
     * 6 src_dev_id
     * 7 dst_dev_id
     */
    return "<2c2H2B";
}

const QByteArray Message::packString(const QVariant& messageIDEnum)
{
    int messageID = messageIDEnum.toInt();
    switch (messageID) {
        // General
        case GeneralMessageID::gen_goto_bootloader :
            /**
             * none
             */
            return "";
        case GeneralMessageID::gen_get_version :
            /**
             * 0 device_type
             * 1 device_model
             * 2-3 fw_version_major
             * 4-5 fw_version_minor
             */
            return "<2B2H";
        case GeneralMessageID::gen_device_id :
            /**
             * 0 device_id
             */
            return "<B";
        case GeneralMessageID::gen_new_data :
            /**
             * 0 is_new_data
             */
            return "<B";
        case GeneralMessageID::gen_cmd_request :
            /**
             * 0-1 request_id
             */
            return "<H";

        // Sonar
        case SonarMessageID::sonar_set_velocity :
            /**
             * 0-3 c_water
             */
            return "<I";

        // Echosounder
        case EchosounderMessageID::es_distance_simple :
            /**
             * 0-3 distance
             * 4 confidence
             */
            return "<IB";

        case EchosounderMessageID::es_distance :
            /**
             * 0-3 distance
             * 4 confidence
             * 5-6 pulse_usec
             * 6 pulse_usec
             * 7-10 ping_number
             * 11-14 start_mm
             * 15-18 length_mm
             * 19-22 gain_index
             * 23-26 gain_index
             */
            return "<IBH4I";
        case EchosounderMessageID::es_profile :
            /**
             * 0-3 distance
             * 4 confidence
             * 5-6 pulse_usec
             * 6 pulse_usec
             * 7-10 ping_number
             * 11-14 start_mm
             * 15-18 length_mm
             * 19-22 gain_index
             * 23-26 gain_index
             * 27-28 num_points
             * 29-n data
             */
            return "<IBH4IH200B";
        case EchosounderMessageID::es_range :
            /**
             * 0-3 start_mm
             * 4-7 length_mm
             */
            return "<2I";
        case EchosounderMessageID::es_mode :
            /**
             * 0 auto_manual
             */
            return "<B";
        case EchosounderMessageID::es_rate :
            /**
             * 0-1 msec_per_ping
             */
            return "<H";
        case EchosounderMessageID::es_gain :
            /**
             * 0 gain_index
             */
            return "<B";
        case EchosounderMessageID::es_pulse :
            /**
             * 0-1 pulse_usec
             */
            return "<H";

        // Mechanical Scanning
        case MechanicalScanningSonarMessageID::mss_angle_profile :
            /**
             * 0-1 angle
             * 2-3 pulse_usec
             * 6-9 range_mm
             * 10-13 gain_index
             * 11-13 num_points
             * 14-n data
             */
            return "<2H2IHXB";
        case MechanicalScanningSonarMessageID::mss_range :
            /**
             * 0-3 range_mm
             */
            return "<I";
        case MechanicalScanningSonarMessageID::mss_mode :
            /**
             * 0 auto_manual
             */
            return "<B";
        case MechanicalScanningSonarMessageID::mss_gain :
            /**
             * 0 gain_index
             */
            return "<B";
        case MechanicalScanningSonarMessageID::mss_sector :
            /**
             * 0-1 train_angle
             * 2-3 sector_width
             * 4 step_size
             * 4-5 pulse_length
             * 5-6 sample_size
             */
            return "<iHB2I";

        default:
            return QByteArray();
    }
}

const QString Message::string(const QVariant& messageIDEnum)
{
    int messageID = messageIDEnum.toInt();
    switch (messageID) {
        // General
        case GeneralMessageID::gen_goto_bootloader :
            return tr("Used by firmware updater only.");
        case GeneralMessageID::gen_get_version :
            return tr("Device information: device type, model and firmware version.");
        case GeneralMessageID::gen_device_id :
            return tr("Device ID.");
        case GeneralMessageID::gen_new_data :
            return tr("Data available.");
        case GeneralMessageID::gen_cmd_request :
            return tr("Request message ID.");

        // Sonar
        case SonarMessageID::sonar_set_velocity :
            return tr("Not supported yet, requires f/w work.");

        // Echosounder
        case EchosounderMessageID::es_distance_simple :
            return tr("Simple distance information.");
        case EchosounderMessageID::es_distance :
            return tr("Distance information.");
        case EchosounderMessageID::es_profile :
            return tr("Profile.");
        case EchosounderMessageID::es_range :
            return tr("Range.");
        case EchosounderMessageID::es_mode :
            return tr("Mode.");
        case EchosounderMessageID::es_rate :
            return tr("Ping rate.");
        case EchosounderMessageID::es_gain :
            return tr("Gain.");
        case EchosounderMessageID::es_pulse :
            return tr("Pulse information.");

        // Mechanical Scanning
        case MechanicalScanningSonarMessageID::mss_angle_profile :
            return tr("Profile.");
        case MechanicalScanningSonarMessageID::mss_range :
            return tr("Range.");
        case MechanicalScanningSonarMessageID::mss_mode :
            return tr("Mode.");
        case MechanicalScanningSonarMessageID::mss_gain :
            return tr("Gain.");
        case MechanicalScanningSonarMessageID::mss_sector :
            return tr("Sector.");

        default:
            return tr("Command do not exist.");
    }
}