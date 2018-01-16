#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>

#include "abstractlink.h"

class SerialLink : public AbstractLink, QSerialPort, QSerialPortInfo
{

public:
    SerialLink();
    ~SerialLink();

    bool setConfiguration(const QString& arg) final;
    bool startConnection() final;
    bool finishConnection() final;
    QStringList listAvailableConnections() final;
};