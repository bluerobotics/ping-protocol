#pragma once

#include "link.h"
#include "parsers/parser.h"
#include "protocol.h"

// TODO rename to Device?
class Sensor : public QObject
{
    Q_OBJECT
public:
    Sensor();
    ~Sensor();

    Q_PROPERTY(Protocol* protocol READ protocol NOTIFY protocolUpdate) // deprecated
    Protocol* protocol() { return _protocol; }; // deprecated

    Q_PROPERTY(AbstractLink* link READ link NOTIFY linkUpdate)
    AbstractLink* link() { return _linkIn->self(); };

    Q_PROPERTY(AbstractLink* linkLog READ linkLog NOTIFY linkLogUpdate)
    AbstractLink* linkLog() { return _linkOut->self(); };

    Q_PROPERTY(QString name READ name NOTIFY nameUpdate)
    QString name() { return _name; };

    Q_INVOKABLE void connectLink(const QString& connString);
    Q_INVOKABLE void connectLinkLog(const QString& connString);

protected:
    Link* _linkIn;
    Link* _linkOut;
    Protocol* _protocol; // deprecated
    Parser* _parser; // communication implementation

    QString _name; // TODO populate

signals:
    // In
    void connectionClose();
    void connectionOpen();
    void protocolUpdate();
    void nameUpdate();
    void linkUpdate();

    // Out
    void linkLogUpdate();
};
