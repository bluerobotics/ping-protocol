#pragma once

#include <QObject>

#include "parser.h"

class JsonParser : public Parser
{
    Q_OBJECT
public:
    JsonParser() = default;
    ~JsonParser() = default;

    typedef enum {
        WaitStart = 0,
        WaitEnd
    } state_t;

    state_t state;

    uint8_t parseByte(const char byte) override;

    void parseBuffer(const QByteArray& buf) override;

signals:
    void newJsonObject(const QJsonObject& obj);
};


