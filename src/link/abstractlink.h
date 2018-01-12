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

    const AbstractLink& operator=(const AbstractLink& other);

    virtual bool setConfiguration(const QStringList& args) { Q_UNUSED(args) return true; }

    void setName(const QString& name) { _name = name; emit nameChanged(_name); };
    const QString name() { return _name; }
    virtual void setType(LinkType type) { _type = type; emit linkChanged(_type); };
    LinkType type() { return _type; };

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
    LinkType _type;
    QString _name;
};