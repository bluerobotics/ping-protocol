#pragma once

#include <QObject>
#include <QTime>

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
        PingSimulation
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

    // Control offline source (E.g: Files)
    Q_INVOKABLE virtual bool isOnline() { return true; };
    Q_INVOKABLE virtual void start() {};
    Q_INVOKABLE virtual void pause() {};
    Q_PROPERTY(qint64 byteSize READ byteSize NOTIFY byteSizeChanged)
    Q_INVOKABLE virtual qint64 byteSize() { return 0; };
    Q_PROPERTY(int packageSize READ packageSize NOTIFY packageSizeChanged)
    Q_INVOKABLE virtual int packageSize() { return 0; };
    Q_PROPERTY(int packageIndex READ packageIndex WRITE setPackageIndex NOTIFY packageIndexChanged)
    Q_INVOKABLE virtual int packageIndex() { return 0; };
    Q_INVOKABLE virtual void setPackageIndex(int index) { Q_UNUSED(index) };
    Q_PROPERTY(QTime totalTime READ totalTime NOTIFY totalTimeChanged)
    Q_INVOKABLE virtual QTime totalTime() { return QTime(); };
    Q_PROPERTY(QTime elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)
    Q_INVOKABLE virtual QTime elapsedTime() { return QTime(); };
    Q_PROPERTY(QString totalTimeString READ totalTimeString NOTIFY totalTimeChanged)
    Q_INVOKABLE virtual QString totalTimeString() { return totalTime().toString(_timeFormat); };
    Q_PROPERTY(QString elapsedTimeString READ elapsedTimeString NOTIFY elapsedTimeChanged)
    Q_INVOKABLE virtual QString elapsedTimeString() { return elapsedTime().toString(_timeFormat); };

    virtual QString errorString() { return QString();};

signals:
    void availableConnectionsChanged();
    void nameChanged(const QString& name);
    void autoConnectChanged();
    void linkChanged(LinkType link);
    void newData(const QByteArray& data);
    void sendData(const QByteArray& data);

    void byteSizeChanged();
    void packageSizeChanged();
    void packageIndexChanged();
    void totalTimeChanged();
    void elapsedTimeChanged();

protected:
    static const QString _timeFormat;

private:
    bool _autoConnect;
    LinkType _type;
    QString _name;
};
