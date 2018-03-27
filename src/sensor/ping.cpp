#include "ping.h"
#include "pingmessage/pingmessage.h"
#include "pingmessage/pingmessage_es.h"
#include "pingmessage/pingmessage_gen.h"
#include "../link/seriallink.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QThread>
#include <QUrl>

void Ping::handleMessage(PingMessage msg)
{
    qDebug() << "Handling Message:" << msg.message_id() << "Checksum Pass:" << msg.verifyChecksum();

    switch (msg.message_id()) {

    case PingMessage::gen_get_version: {
        ping_msg_gen_version m(msg);
        _device_type = m.device_type();
        _device_model = m.device_model();
        _fw_version_major = m.fw_version_major();
        _fw_version_minor = m.fw_version_minor();

        emit deviceTypeUpdate();
        emit deviceModelUpdate();
        emit fwVersionMajorUpdate();
        emit fwVersionMinorUpdate();
    }
        break;

    case PingMessage::es_distance: {
        ping_msg_es_distance m(msg);
        _distance = m.distance();
        _confidence = m.confidence();
        _pulse_usec = m.pulse_usec();
        _ping_number = m.ping_number();
        _start_mm = m.start_mm();
        _length_mm = m.length_mm();
        _gain_index = m.gain_index();

        // TODO, change to distMsgUpdate() or similar
        emit distanceUpdate();
        emit pingNumberUpdate();
        emit confidenceUpdate();
        emit pulseUsecUpdate();
        emit startMmUpdate();
        emit lengthMmUpdate();
        emit gainIndexUpdate();
    }
        break;

    case PingMessage::es_profile: {
        ping_msg_es_profile m(msg);
        _distance = m.distance();
        _confidence = m.confidence();
        _pulse_usec = m.pulse_usec();
        _ping_number = m.ping_number();
        _start_mm = m.start_mm();
        _length_mm = m.length_mm();
        _gain_index = m.gain_index();
//        num_points = m.num_points(); // const
//        memcpy(_points.data(), m.data(), _num_points); // careful with constant

        // This is necessary to convert <uint8_t> to <int>
        // QProperty only supports vector<int>, otherwise, we could use memcpy
        for (int i = 0; i < m.num_points(); i++) {
            _points.replace(i, m.data()[i] / 255.0); // TODO we should really be working in ints
        }

        // TODO, change to distMsgUpdate() or similar
        emit distanceUpdate();
        emit pingNumberUpdate();
        emit confidenceUpdate();
        emit pulseUsecUpdate();
        emit startMmUpdate();
        emit lengthMmUpdate();
        emit gainIndexUpdate();
        emit pointsUpdate();
    }
        break;

    case PingMessage::es_mode: {
        ping_msg_es_mode m(msg);
        _mode_auto = m.auto_manual();
        emit modeAutoUpdate();
    }
        break;

    default:
        qCritical() << "UNHANDLED MESSAGE ID:" << msg.message_id();
        break;
    }

    _dstId = msg.dst_device_id();
    _srcId = msg.src_device_id();

    emit dstIdUpdate();
    emit srcIdUpdate();

//    printStatus();
}

void Ping::firmwareUpdate(QString fileUrl)
{
    SerialLink* serialLink = dynamic_cast<SerialLink*>(link());

    if (!serialLink) {
        return;
    }

    if(!link()->isOpen()) {
        return;
    }

    qDebug() << "Put it in bootloader mode.";
    ping_msg_gen_goto_bootloader m;
    m.updateChecksum();
    link()->sendData(QByteArray(reinterpret_cast<const char*>(m.msgData.data()), m.msgData.size()));

    while (serialLink->QSerialPort::bytesToWrite()) {
        qDebug() << "Waiting for bytes to be written...";
        serialLink->QSerialPort::waitForBytesWritten();
        qDebug() << "Done !";
    }

    qDebug() << "Finish connection.";
    // TODO: Move thread delay to something more.. correct.
    QThread::usleep(500e3);
    link()->finishConnection();


    QSerialPortInfo pInfo(serialLink->QSerialPort::portName());
    QString portLocation = pInfo.systemLocation();

    auto flash = [=](const QString& portLocation, const QString& firmwareFile, bool verify = false /*verify*/) {
        #ifdef Q_OS_OSX
        // macdeployqt file do not put stm32flash binary in the same folder of pingviewer
        static QString binPath = QCoreApplication::applicationDirPath() + "/../..";
        #else
        static QString binPath = QCoreApplication::applicationDirPath();
        #endif
        static QString cmd = binPath + "/stm32flash -w %0 %1 -v -g 0x0";

        QProcess *process = new QProcess();
        process->setEnvironment(QProcess::systemEnvironment());
        process->setProcessChannelMode(QProcess::MergedChannels);
        qDebug() << "3... 2... 1...";
        qDebug() << cmd.arg(QFileInfo(firmwareFile).absoluteFilePath(), portLocation);
        process->start(cmd.arg(QFileInfo(firmwareFile).absoluteFilePath(), portLocation));
        emit flashProgress(0);
        connect(process, &QProcess::readyReadStandardOutput, this, [this, process] {
            QString output(process->readAllStandardOutput());
            // Track values like: (12.23%)
            QRegularExpression regex("\\d{1,3}[.]\\d\\d");
            QRegularExpressionMatch match = regex.match(output);
            if (match.hasMatch()) {
                QStringList percs = match.capturedTexts();
                for(const auto& perc : percs) {
                    _fw_update_perc = perc.toFloat();

                    if (_fw_update_perc > 99.99) {
                        emit flashComplete();
                    } else {
                        emit flashProgress(_fw_update_perc);
                    }
                }
            }

            qDebug() << output;
        });
    };

    qDebug() << "Start flash.";
    QThread::usleep(500e3);
    flash(portLocation, QUrl(fileUrl).toLocalFile());
}

void Ping::request(int id)
{
    qDebug() << "Requesting:" << id;

    ping_msg_gen_cmd_request m;
    m.set_request_id(id);
    m.updateChecksum();

    writeMessage(m);
}

void Ping::printStatus()
{
    qDebug() << "Ping Status:";
    qDebug() << "\t- srcId:" << _srcId;
    qDebug() << "\t- dstID:" << _dstId;
    qDebug() << "\t- device_type:" << _device_type;
    qDebug() << "\t- device_model:" << _device_model;
    qDebug() << "\t- fw_version_major:" << _fw_version_major;
    qDebug() << "\t- fw_version_minor:" << _fw_version_minor;
    qDebug() << "\t- distance:" << _distance;
    qDebug() << "\t- confidence:" << _confidence;
    qDebug() << "\t- pulse_usec:" << _pulse_usec;
    qDebug() << "\t- ping_number:" << _ping_number;
    qDebug() << "\t- start_mm:" << _start_mm;
    qDebug() << "\t- length_mm:" << _length_mm;
    qDebug() << "\t- gain_index:" << _gain_index;
    qDebug() << "\t- mode_auto:" << _mode_auto;
    qDebug() << "\t- msec_per_ping:" << _msec_per_ping;
//    qDebug() << "\t- points:" << QByteArray((const char*)points, num_points).toHex();
}

void Ping::writeMessage(const PingMessage &msg)
{
    // todo add link::write(char*, int size)
    link()->sendData(QByteArray(reinterpret_cast<const char*>(msg.msgData.data()), msg.msgData.size()));
}
