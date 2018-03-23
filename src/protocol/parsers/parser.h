

#pragma once

#include <QObject>

// This class digests data and notifies owner when something interesting happens
class Parser : public QObject
{
    Q_OBJECT
public:
    Parser();
    ~Parser();

    uint32_t parsed; // number of messages/packets successfully parsed
    uint32_t errors; // number of parse errors

    virtual void parseBuffer(const QByteArray& data) { Q_UNUSED(data)} // asynchronous use, Child classes should signal when something happens ie. 'emit newMessage(Message m)'
    virtual uint8_t parseByte(const char& byte) { Q_UNUSED(byte) return 0; } // synchronous use, Child should return flags indicating incremental parse result/status
};

inline Parser::Parser()
{
}

inline Parser::~Parser()
{
}
