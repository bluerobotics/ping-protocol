#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>

#include "parser_json.h"
#include "parser.h"

Q_LOGGING_CATEGORY(PING_PROTOCOL_JSONPARSER, "ping.protocol.jsonparser")

uint8_t JsonParser::parseByte(const char& byte)
{
    static QByteArray buf;
    switch(state) {
    case WaitStart:
        if (byte == '{') {
            buf.append(byte);
            state = WaitEnd;
        } else {
            errors++;
        }
        break;
    case WaitEnd:
        buf.append(byte);
        if (byte == '}') {
            QJsonDocument doc = QJsonDocument::fromJson(buf);
            if (doc.isNull()) {
                // error
                qCDebug(PING_PROTOCOL_JSONPARSER) << "Error parsing JSON document from buffer";
                errors++;
            } else {
                if (!doc.isObject()) {
                    qCDebug(PING_PROTOCOL_JSONPARSER) << "Error JSON document is not object";
                    errors++;
                } else {
                    parsed++;
                    emit newJsonObject(doc.object());
                }
            }
            state = WaitStart;
            buf.clear();
        }
        break;
    }
    return 0;
}

void JsonParser::parseBuffer(const QByteArray& buf)
{
    for (int i = 0; i < buf.size(); i++) {
        parseByte(buf.at(i));
    }
}


