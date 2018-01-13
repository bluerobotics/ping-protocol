#pragma once

#include <QObject>

#include "message.h"
#include "packer.h"

class Protocol : public QObject
{
    Q_OBJECT
public:
    Protocol();
    ~Protocol();

    Message _message;
    Packer _packer;

    void handleData(const QByteArray& data);
    void requestVersion();

signals:
    void sendData(const QByteArray& data);
};