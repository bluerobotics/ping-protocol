#pragma once

#include <QObject>

class AbstractLink : public QObject
{
    Q_OBJECT
    Q_ENUMS(LinkType)
public:
    AbstractLink();
    ~AbstractLink();

    enum LinkType {
        None = 0,
        File,
        Serial,
        Udp,
        Tcp,
    };

    void setName(const QString& name) { _name = name; emit nameChanged(_name); };
    const QString name() { return _name; }
    virtual void setType(LinkType type) { _linkType = type; emit linkChanged(_linkType); };
    LinkType type() { return _linkType; };

    bool isAutoConnect() { return _autoConnect; }
    virtual void setAutoConnect(bool autoc = true) { _autoConnect = autoc; emit autoConnectChanged(); }

    virtual bool connect() { return true;};
    virtual bool disconnect() { return true;};

signals:
    void nameChanged        (const QString& name);
    void autoConnectChanged ();
    void linkChanged        (LinkType link);

private:
    bool _autoConnect;
    LinkType _linkType;
    QString _name;
};