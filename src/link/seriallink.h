#pragma once

#include <QSerialPort>

#include "abstractlink.h"

class SerialLink : public AbstractLink, QSerialPort
{

public:
    SerialLink();
    ~SerialLink();

    bool setConfiguration(const QStringList& args) final;
    bool startConnection() final;
    bool finishConnection() final;
};