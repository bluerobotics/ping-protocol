

#pragma once

#include <QObject>
#include "pingmessage/ping_message.h"

/**
 * @brief Parser that digests data and notifies owner when something interesting happens
 *
 */
class Parser : public QObject
{
    Q_OBJECT
public:
    Parser() = default;
    ~Parser() = default;

    /**
     * @brief Number of messages/packets successfully parsed
     *
     */
    uint32_t parsed;

    /**
     * @brief Number of parse errors
     *
     */
    uint32_t errors;

    /**
     * @brief Parse a buffer
     *  Should be used asynchronously.
     *  The child classes emit `newMessage`
     * @param data
     */
    virtual void parseBuffer(const QByteArray& data) = 0;

    /**
     * @brief Parse a single byte
     *
     * @param byte
     * @return uint8_t
     */
    virtual uint8_t parseByte(const char byte) = 0;

    /**
     * @brief Return the last valid ping message
     *
     * @return ping_message
     */
    virtual ping_message pingMessage() const = 0;

signals:
    void newMessage(const ping_message& msg);
};
