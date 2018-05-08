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

Ping::Ping() : Sensor() {
    _points.reserve(_num_points);
    for (int i = 0; i < _num_points; i++) {
        _points.append(0);
    }
    _parser = new PingParser();
    connect(dynamic_cast<PingParser*>(_parser), &PingParser::newMessage, this, &Ping::handleMessage);
    connect(link(), &AbstractLink::newData, _parser, &Parser::parseBuffer);
    emit linkUpdate();

    _requestTimer.setInterval(1000);
    connect(&_requestTimer, &QTimer::timeout, this, [this]{ request(PingMessage::es_profile); });

    //connectLink("2:/dev/ttyUSB2:115200");

    connect(&_detector, &ProtocolDetector::_detected, this, &Ping::connectLink);
    _detector.start();

    connect(this, &Ping::autoDetectUpdate, this, [this](bool autodetect){
        if(!autodetect) {
            if(_detector.isRunning()) {
                _detector.exit();
            }
        } else {
            if(!_detector.isRunning()) {
                _detector.start();
            }
        }
    });
}

void Ping::connectLink(const QString& connString)
{
    if(_detector.isRunning()) {
        _detector.exit();
    }
    setAutoDetect(false);
    Sensor::connectLink(connString);
}

void Ping::handleMessage(PingMessage msg)
{
    qDebug() << "Handling Message:" << msg.message_id() << "Checksum Pass:" << msg.verifyChecksum();

    switch (msg.message_id()) {

    case PingMessage::gen_version: {
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

    case PingMessage::es_rate: {
        ping_msg_es_rate m(msg);
        _msec_per_ping = m.msec_per_ping();
        emit msecPerPingUpdate();
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

void Ping::firmwareUpdate(QString fileUrl, bool sendPingGotoBootloader)
{
    SerialLink* serialLink = dynamic_cast<SerialLink*>(link());

    if (!serialLink) {
        return;
    }

    if(!link()->isOpen()) {
        return;
    }

    setPollFrequency(0);

    if (sendPingGotoBootloader) {
        qDebug() << "Put it in bootloader mode.";
        ping_msg_gen_goto_bootloader m;
        m.updateChecksum();
        link()->sendData(QByteArray(reinterpret_cast<const char*>(m.msgData.data()), m.msgData.size()));
    }

    // Wait for bytes to be written before finishing the connection
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

QVariant Ping::pollFrequency()
{
    if (!_requestTimer.isActive()) {
        return 0;
    }
    return 1000.0f / _requestTimer.interval();
}

void Ping::setPollFrequency(QVariant pollFrequency)
{
    if (pollFrequency.toInt() <= 0) {
        if (_requestTimer.isActive()) {
            _requestTimer.stop();
        }
    } else {
        int period_ms = 1000.0f / pollFrequency.toInt();
        qDebug() << "setting f" << pollFrequency.toInt() << period_ms;
        _requestTimer.setInterval(period_ms);
        if (!_requestTimer.isActive()) {
            _requestTimer.start();
        }

        set_msec_per_ping(period_ms);
    }

    qDebug() << "Poll period" << pollFrequency;
    emit pollFrequencyUpdate();
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