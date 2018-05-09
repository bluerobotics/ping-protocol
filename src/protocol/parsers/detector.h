#pragma once

#include <QThread>

#include "parser_ping.h"

/// This class will scan network ports and serial ports for a ping device
/// TODO subclass and support discovery of other protocols/devices
class ProtocolDetector : public QThread
{
    Q_OBJECT
public:
    ProtocolDetector(){
        connect(this, &QThread::finished, [this]{ _active = false; });
    };

    void scan();

signals:
    void _detected(QString config); // Todo can we send a pre-configured, pre-connected link object with the signal?

protected:
    void run() { scan(); }

private:
    bool _active { false };
    PingParser _parser;
};
