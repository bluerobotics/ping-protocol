#pragma once

#include <QSerialPort>

#include "abstractlink.h"

class SerialLink : public AbstractLink, QSerialPort
{

public:
    SerialLink();
    ~SerialLink();

    bool setConfiguration(const QString& arg) final;
    bool startConnection() final;
    bool finishConnection() final;
};