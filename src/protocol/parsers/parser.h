

#pragma once

#include <QObject>
#include "pingmessage/ping_message.h"

// This class digests data and notifies owner when something interesting happens
class Parser : public QObject
{
    Q_OBJECT
public:
    Parser() = default;
    ~Parser() = default;

    uint32_t parsed; // number of messages/packets successfully parsed
    uint32_t errors; // number of parse errors

    virtual void parseBuffer(const QByteArray& data) = 0; // asynchronous use, Child classes should signal when something happens ie. 'emit newMessage(Message m)'
    virtual uint8_t parseByte(const char byte) = 0; // synchronous use, Child should return flags indicating incremental parse result/status

    /**
     * @brief Return the last valid ping message
     *
     * @return ping_message
     */
    virtual ping_message pingMessage() const = 0;
};
