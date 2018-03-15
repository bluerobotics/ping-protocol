#pragma once

#include "link.h"
#include "protocol.h"
#include "stm32flasher.h"

class Ping : public QObject
{
    Q_OBJECT
public:
    Ping();
    ~Ping();

    Q_PROPERTY(Protocol* protocol READ protocol NOTIFY protocolUpdate)
    Protocol* protocol() { return _protocol; };

    Q_PROPERTY(AbstractLink* link READ link NOTIFY linkUpdate)
    AbstractLink* link() { return _linkIn->self(); };

    Q_PROPERTY(QVariant pollFrequency READ pollFrequency WRITE setPollFrequency NOTIFY pollFrequencyUpdate)
    QVariant pollFrequency();
    void setPollFrequency(QVariant pollFrequency);

    Q_PROPERTY(AbstractLink* linkLog READ linkLog NOTIFY linkLogUpdate)
    AbstractLink* linkLog() { return _linkOut->self(); };

    Q_INVOKABLE void connectLink(const QString& connString);
    void reconnectLink(bool something);
    Q_INVOKABLE void connectLinkLog(const QString& connString);

    Q_INVOKABLE void firmwareUpdate(const QUrl& fileUrl);

signals:
    void flashProgress(float progress);
    void flashComplete(bool success); // TODO make use of success flag

private:
    QString _linkConfigString;
    Link* _linkIn;
    Link* _linkOut;
    Protocol* _protocol;
    QTimer requestTimer;
    Stm32Flasher _flasher;

signals:
    // In
    void connectionClose();
    void connectionOpen();
    void protocolUpdate();
    void linkUpdate();

    // Out
    void linkLogUpdate();
    void pollFrequencyUpdate();
};
