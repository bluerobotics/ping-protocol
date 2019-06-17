#include "parser_ping.h"

void operator++(PingParser::State& state, int) {
    state = static_cast<PingParser::State>((static_cast<int>(state)) + 1);
};

void PingParser::parseBuffer(const QByteArray& data)
{
    for(int i = 0; i < data.length(); i++) {
        parseByte(data.at(i));
    }
}

bool PingParser::parseByte(const char byte)
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
            _state = State::WAIT_START;
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
        _lastMsg = ping_message((uint8_t*)_parseBuf.data(), _parseBuf.length());
        _payloadLength = 0;
        _msgId = 0;
        _state = State::WAIT_START;

        if (_lastMsg.verifyChecksum()) {
            _parseBuf.clear();
            emit newMessage(_lastMsg);
            parsed++;
            return true;
        }
        _parseBuf.clear();

        errors++;
        emit parseError();
    }
    return false;
}
