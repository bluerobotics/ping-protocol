#pragma once

#include <QDebug>
#include <QNetworkDatagram>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QUdpSocket>

#include "parser_ping.h"
#include "pingmessage/pingmessage.h"
#include "pingmessage/pingmessage_gen.h"

/// This class will scan network ports and serial ports for a ping device
/// TODO subclass and support discovery of other protocols/devices
class ProtocolDetector : public QThread
{
    Q_OBJECT
public:
    ProtocolDetector() {};
    ~ProtocolDetector() {};

    void scan() {
        _active = true;

        // To find a ping, we this message on a link, then wait for a reply
        ping_msg_gen_cmd_request req;
        req.set_request_id(PingMessage::gen_get_version);
        req.updateChecksum();

        // Scan until we find a ping, then stop
        while (_active) {

            // Try BlueROV2 standard udp port first
            QUdpSocket sockit;

            // To test locally, change the host to 127.0.0.1 and use something like:
            // nc -kul 127.0.0.1 8888 > /dev/ttyUSB0 < /dev/ttyUSB0
            auto _hostAddress = QHostAddress("192.168.2.2");
            int _port = 9090;

            qDebug() << "Probing UDP" << _hostAddress.toString() << _port;
            // sockit.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
            sockit.writeDatagram(reinterpret_cast<const char*>(req.msgData.data()), req.msgData.size(), _hostAddress, _port); // probe

            bool detected = false;
            int attempts = 0;

            while (!detected && attempts < 10) { // Try to get a valid response, timeout after 40 ms
                sockit.waitForReadyRead(4);
                auto datagram = sockit.receiveDatagram();
                auto buf = datagram.data();
                for (auto byte = buf.begin(); byte != buf.end(); ++byte) {
                    detected = _parser.parseByte(*byte) == PingParser::NEW_MESSAGE;
                    if (detected) {
                        break;
                    }
                }
                attempts++;
            }

            sockit.close();

            if (detected) {
                qDebug() << "Ping detected on UDP";
                emit _detected(QString("3:%1:%2").arg(_hostAddress.toString()).arg(_port));
                _active = false;
                return;
            }

            // Not found on UDP, now try all available serial ports
            auto ports = QSerialPortInfo::availablePorts();

            for (auto it = ports.begin(); it != ports.end(); ++it) { // Scan all available ports
                qDebug() << "probing" << it->portName();

                QSerialPort p(*it);

                if (p.open(QIODevice::ReadWrite)) { // Attempt to open port
                    p.setBaudRate(115200);

                    // Probe
                    p.write(reinterpret_cast<const char*>(req.msgData.data()), (uint16_t)req.msgData.size());
                    p.waitForBytesWritten();

                    bool detected = false;
                    int attempts = 0;

                    while (!detected && attempts < 10) { // Try to get a valid response, timeout after 40 ms
                        p.waitForReadyRead(4);
                        auto buf = p.readAll();
                        for (auto byte = buf.begin(); byte != buf.end(); ++byte) {
                            detected = _parser.parseByte(*byte) == PingParser::NEW_MESSAGE;
                            if (detected) {
                                break;
                            }
                        }
                        attempts++;
                    }

                    p.close();

                    if (detected) {
                        qDebug() << "Ping detected on" << p.portName();
                        emit _detected("2:/dev/" + p.portName() + ":115200");
                        _active = false;
                        return;
                    }
                } else {
                    qDebug() << "couldn't open port";
                }
            }
            msleep(350);
        }
    }

signals:
    void _detected(QString config); // Todo can we send a pre-configured, pre-connected link object with the signal?

protected:
    void run() { scan(); }

private:
    bool _active { false };
    PingParser _parser;
};
