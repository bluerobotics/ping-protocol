#pragma once

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

#include "parser.h"

class JsonParser : public Parser
{
    Q_OBJECT
public:
    JsonParser() {}
    ~JsonParser() {}

    typedef enum {
        WaitStart = 0,
        WaitEnd
    } state_t;

    state_t state;

    uint8_t parseByte(const char& byte) override
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
                    qDebug() << "Error parsing JSON document from buffer";
                    errors++;
                } else {
                    if (!doc.isObject()) {
                        qDebug() << "Error JSON document is not object";
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

    void parseBuffer(const QByteArray& buf) override {
        for (int i = 0; i < buf.size(); i++) {
            parseByte(buf.at(i));
        }
    }

signals:
    void newJsonObject(const QJsonObject& obj);
};


