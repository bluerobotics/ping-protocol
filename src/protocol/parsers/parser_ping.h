#pragma once

#include <QDebug>
#include "parser.h"
#include "pingmessage/pingmessage.h"

class PingParser : public Parser
{
    Q_OBJECT
public:
    PingParser() = default;
    ~PingParser() = default;

    enum {
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
        static QByteArray parseBuf;
        static uint16_t payload_length = 0;
        static uint16_t msg_id = 0; // debug purposes only
        static uint8_t state = WAIT_START;

        switch(state) {
        case WAIT_START:
            if (byte == 'B') {
                parseBuf.append(byte);
                state++;
            }
            break;
        case WAIT_HEADER:
            if (byte == 'R') {
                parseBuf.append(byte);
                state++;
            } else {
                parseBuf.clear();
                state = WAIT_START;
            }
            break;
        case WAIT_LENGTH_L:
            parseBuf.append(byte);
            payload_length = (uint8_t)byte;
            state++;
            break;
        case WAIT_LENGTH_H:
            parseBuf.append(byte);
            payload_length = (byte << 8) | payload_length;
            state++;
            break;
        case WAIT_MSG_ID_L:
            parseBuf.append(byte);
            msg_id = (uint8_t)byte;
            state++;
            break;
        case WAIT_MSG_ID_H:
            parseBuf.append(byte);
            msg_id = (byte << 8) | msg_id;
            state++;
            break;
        case WAIT_SRC_ID:
            parseBuf.append(byte);
            state++;
            break;
        case WAIT_DST_ID:
            parseBuf.append(byte);
            state++;
            if (payload_length == 0) { // no payload bytes
                state++;
            }
            break;
        case WAIT_PAYLOAD:
            if (payload_length) {
                parseBuf.append(byte);
                payload_length--;
            }
            if (payload_length == 0) {
                state++;
            }
            break;
        case WAIT_CHECKSUM_L:
            parseBuf.append(byte);
            state++;
            break;
        case WAIT_CHECKSUM_H:
            parseBuf.append(byte);
            PingMessage msg((uint8_t*)parseBuf.data(), parseBuf.length());
            bool ok = false;
            if (!msg.verifyChecksum()) {
                errors++;
                emit parseError();
            } else {
                ok = true;
                emit newMessage(msg);
                parsed++;
            }

            parseBuf.clear();
            payload_length = 0;
            msg_id = 0;
            state = WAIT_START;

            // TODO print state of message here after clearing buf
            if (ok) {
                return NEW_MESSAGE;
            }
            return state;
        }
        return state;
    }

signals:
    void newMessage(PingMessage msg);
    void parseError();
};
