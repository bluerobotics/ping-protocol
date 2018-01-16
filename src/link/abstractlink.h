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

    virtual bool setConfiguration(const QString& arg) { Q_UNUSED(arg) return true; }

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    void setName(const QString& name) { _name = name; emit nameChanged(_name); };
    const QString name() { return _name; }

    Q_PROPERTY(LinkType type READ type WRITE setType NOTIFY linkChanged)
    virtual void setType(LinkType type) { _type = type; emit linkChanged(_type); };
    LinkType type() { return _type; };

    Q_PROPERTY(bool isAutoConnect READ isAutoConnect WRITE setAutoConnect NOTIFY autoConnectChanged)
    bool isAutoConnect() { return _autoConnect; }
    virtual void setAutoConnect(bool autoc = true) { _autoConnect = autoc; emit autoConnectChanged(); }

    Q_PROPERTY(QStringList listAvailableConnections READ listAvailableConnections NOTIFY availableConnectionsChanged)
    Q_INVOKABLE virtual QStringList listAvailableConnections() { return QStringList(); };

    Q_INVOKABLE virtual bool startConnection() { return true;};
    Q_INVOKABLE virtual bool finishConnection() { return true;};
    Q_INVOKABLE virtual bool isOpen() { return true;};

signals:
    void availableConnectionsChanged();
    void nameChanged(const QString& name);
    void autoConnectChanged();
    void linkChanged(LinkType link);
    void newData(const QByteArray& data);
    void sendData(const QByteArray& data);

private:
    bool _autoConnect;
    LinkType _type;
    QString _name;
};