#include <QtConcurrent>
#include <QDebug>
#include <QFuture>
#include <QLoggingCategory>
#include <QNetworkDatagram>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QUdpSocket>

#include "detector.h"
#include "pingmessage/pingmessage.h"
#include "pingmessage/pingmessage_ping1D.h"

Q_LOGGING_CATEGORY(PING_PROTOCOL_PROTOCOLDETECTOR, "ping.protocol.protocoldetector")


const QStringList ProtocolDetector::_invalidSerialPortNames(
    {
#ifdef Q_OS_OSX
        "cu.", "SPPDev", "iPhone", "Bluetooth",
#endif
    });

void ProtocolDetector::scan() {
    _active = true;

    // To find a ping, we this message on a link, then wait for a reply
    ping_msg_ping1D_empty req;
    req.set_id(Ping1DNamespace::Fw_version);
    req.updateChecksum();

    // Scan until we find a ping, then stop
    while (_active) {

        // Try BlueROV2 standard udp port first
        QUdpSocket sockit;

        // To test locally, change the host to 127.0.0.1 and use something like:
        // nc -kul 127.0.0.1 8888 > /dev/ttyUSB0 < /dev/ttyUSB0
        auto _hostAddress = QHostAddress("192.168.2.2");
        int _port = 9090;

        qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Probing UDP" << _hostAddress.toString() << _port;
        // sockit.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
        sockit.writeDatagram(reinterpret_cast<const char*>(req.msgData.data()), req.msgData.size(), _hostAddress, _port); // probe

        bool detected = false;
        int attempts = 0;

        while (!detected && attempts < 10) { // Try to get a valid response, timeout after 40 ms
            sockit.waitForReadyRead(50);
            QNetworkDatagram datagram = sockit.receiveDatagram();
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
            qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Ping detected on UDP";
            emit _detected(QString("3:%1:%2").arg(_hostAddress.toString()).arg(_port));
            _active = false;
            return;
        }

        // Not found on UDP, now try all available serial ports
        auto ports = QSerialPortInfo::availablePorts();

        // Scan all available ports
        for (auto& port : ports) {
            // Do not run with invalid ports
            if(!isValidPort(port)) {
                continue;
            }

            // Check if port can be opened
            if(!canOpenPort(port, 500)) {
                qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Couldn't open port" << port.portName();
                continue;
            }

            QSerialPort p(port);

            for (const int baudrate : {115200, 921600}) {
                qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Probing Serial" << port.portName() << baudrate;

                if(!p.open(QIODevice::ReadWrite)) {
                    qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Fail to open";
                    break;
                }
                qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Port is open";
                p.setBaudRate(baudrate);

                // Probe
                p.write(reinterpret_cast<const char*>(req.msgData.data()), (uint16_t)req.msgData.size());
                p.waitForBytesWritten();

                bool detected = false;
                int attempts = 0;

                while (!detected && attempts < 10) { // Try to get a valid response, timeout after 40 ms
                    p.waitForReadyRead(50);
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
                    qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Ping detected on" << p.portName() << baudrate;
                    emit _detected(QString("2:%1:%2").arg(p.portName()).arg(baudrate));
                    _active = false;
                    return;
                } else {
                    qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Couldn't detect ping";
                }
            }
        }
        msleep(500);
    }
}

bool ProtocolDetector::canOpenPort(QSerialPortInfo& port, int msTimeout) {
    // Call function asynchronously:
    auto checkPort = [](const QSerialPortInfo& portInfo){
        QSerialPort port(portInfo);
        bool ok = port.open(QIODevice::ReadWrite);
        // Close will check if is open
        port.close();
        return ok;
    };

    QFuture<bool> future = QtConcurrent::run(checkPort, port);
    // Wait for msTimeout
    float waitForTenthOfTimeout = 0;
    while(waitForTenthOfTimeout < 10 && !future.isFinished()) {
        msleep(msTimeout/10.0f);
        qCDebug(PING_PROTOCOL_PROTOCOLDETECTOR) << "Waiting port to open.. " << waitForTenthOfTimeout << port.portName();
        waitForTenthOfTimeout += 1;
    }

    bool ok = false;
    if(future.isFinished()) {
        ok = future.result();
    }
    return ok;
}

bool ProtocolDetector::isValidPort(QSerialPortInfo& serialPortInfo) {
    for(auto name : _invalidSerialPortNames) {
        if(serialPortInfo.portName().contains(name, Qt::CaseInsensitive)) {
            return false;
        }
    }
    return true;
}
