#pragma once

#include "parser.h"

class PingParser : public Parser
{
    Q_OBJECT
public:
    PingParser() = default;
    ~PingParser() = default;

    enum class State {
        WAIT_START,    // Waiting for the first character of a message 'B'
        WAIT_HEADER,   // Waiting for the second character in the two-character sequence 'BR'
        WAIT_LENGTH_L, // Waiting for the low byte of the payload length field
        WAIT_LENGTH_H, // Waiting for the high byte of the payload length field
        WAIT_MSG_ID_L, // Waiting for the low byte of the payload id field
        WAIT_MSG_ID_H, // Waiting for the high byte of the payload id field
        WAIT_SRC_ID,   // Waiting for the source device id
        WAIT_DST_ID,   // Waiting for the destination device id
        WAIT_PAYLOAD,  // Waiting for the last byte of the payload to come in
        WAIT_CHECKSUM_L, // Waiting for the checksum low byte
        WAIT_CHECKSUM_H, // Waiting for the checksum high byte
    };

    /**
     * @brief Parse a buffer
     *
     * @param data
     */
    void parseBuffer(const QByteArray& data) override;

    /**
     * @brief Parse a single byte
     *
     * @param byte
     * @return bool
     */
    bool parseByte(const char byte) override;

    /**
     * @brief Return the last available message
     *
     * @return ping_message
     */
    virtual ping_message pingMessage() const override {
        return _lastMsg;
    };

signals:
    void parseError();

private:
    QByteArray _parseBuf;
    ping_message _lastMsg;
    uint16_t _msgId = 0; // debug purposes only
    uint16_t _payloadLength = 0;
    State _state = State::WAIT_START;
};
