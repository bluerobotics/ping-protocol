#pragma once

#include <QObject>

class Message : public QObject
{
    Q_OBJECT
    Q_ENUMS(GeneralMessageID)
    Q_ENUMS(SonarMessagesID)
    Q_ENUMS(EchosounderMessageID)
public:
    Message();
    ~Message();

    enum GeneralMessageID {
         gen_goto_bootloader = 100
        ,gen_get_version = 101
        ,gen_device_id = 110
        ,gen_new_data = 112
        ,gen_cmd_request = 120
    };

    enum SonarMessageID {
         sonar_set_velocity = 1000
    };

    enum EchosounderMessageID {
         es_distance_simple = 1100
        ,es_distance = 1101
        ,es_profile
        ,es_range = 1110
        ,es_mode
        ,es_rate
        ,es_gain
        ,es_pulse
    };

    enum MechanicalScanningSonarMessageID {
         mss_angle_profile = 1201
        ,mss_range = 1210
        ,mss_mode
        ,mss_gain
        ,mss_sector
    };

    static const QByteArray checksumPackString();
    static const QByteArray headerPackString();
    static const QByteArray packString(const QVariant& messageIDEnum);
    static const QString string(const QVariant& messageIDEnum);
};