#pragma once

#include <QDebug>
#include "parser.h"
#include "pingmessage/ping_ping1d.h"

class PingParser : public Parser
{
    Q_OBJECT
public:
    PingParser() = default;
    ~PingParser() = default;

    enum class State {
        NEW_MESSAGE,   // Just got a complete checksum-verified message
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

    void parseBuffer(const QByteArray& data) override
    {
        for(int i = 0; i < data.length(); i++) {
            parseByte(data.at(i));
        }
    }

    uint8_t parseByte(const char byte) override
    {
        switch(_state) {
        case State::WAIT_START:
            if (byte == 'B') {
                _parseBuf.append(byte);
                _state++;
            }
            break;
        case State::WAIT_HEADER:
            if (byte == 'R') {
                _parseBuf.append(byte);
                _state++;
            } else {
                _parseBuf.clear();
                _state = WAIT_START;
            }
            break;
        case State::WAIT_LENGTH_L:
            _parseBuf.append(byte);
            _payloadLength = (uint8_t)byte;
            _state++;
            break;
        case State::WAIT_LENGTH_H:
            _parseBuf.append(byte);
            _payloadLength = (byte << 8) | _payloadLength;
            _state++;
            break;
        case State::WAIT_MSG_ID_L:
            _parseBuf.append(byte);
            _msgId = (uint8_t)byte;
            _state++;
            break;
        case State::WAIT_MSG_ID_H:
            _parseBuf.append(byte);
            _msgId = (byte << 8) | _msgId;
            _state++;
            break;
        case State::WAIT_SRC_ID:
            _parseBuf.append(byte);
            _state++;
            break;
        case State::WAIT_DST_ID:
            _parseBuf.append(byte);
            _state++;
            if (_payloadLength == 0) { // no payload bytes
                _state++;
            }
            break;
        case State::WAIT_PAYLOAD:
            if (_payloadLength) {
                _parseBuf.append(byte);
                _payloadLength--;
            }
            if (_payloadLength == 0) {
                _state++;
            }
            break;
        case State::WAIT_CHECKSUM_L:
            _parseBuf.append(byte);
            _state++;
            break;
        case State::WAIT_CHECKSUM_H:
            _parseBuf.append(byte);
            ping_message msg((uint8_t*)_parseBuf.data(), _parseBuf.length());
            bool ok = false;
            if (!msg.verifyChecksum()) {
                errors++;
                emit parseError();
            } else {
                ok = true;
                emit newMessage(msg);
                parsed++;
            }

            _parseBuf.clear();
            _payloadLength = 0;
            _msgId = 0;
            _state = WAIT_START;

            // TODO print state of message here after clearing buf
            if (ok) {
                return NEW_MESSAGE;
            }
            return _state;
        }
        return _state;
    }

signals:
    void newMessage(ping_message msg);
    void parseError();

private:
    uint16_t _msgId = 0; // debug purposes only
    QByteArray _parseBuf;
    uint16_t _payloadLength = 0;
    uint8_t _state = WAIT_START;
};
