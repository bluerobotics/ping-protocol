#pragma once

#include <QObject>
#include <QVariantList>

class Packer : public QObject
{
    Q_OBJECT
public:
    Packer();
    ~Packer();

    void decode(QByteArray data);
    QByteArray request(const QVariant& messageID, int srcDevID = 0, int dstDevID = 0);

private:
    bool validadeData(QByteArray& data, QVariantList& package);
    QString checkPackString(const QString& packString);
    QVariantList unpack(const QString& packString, QByteArray data);
    QVariant unconvert(QByteArray& data, const QChar& format);
    QByteArray messagePack(const QVariant& messageID, const QVariant& var);
    QByteArray messagePack(const QVariant& messageID, const QVariantList& var);
    QByteArray pack(const QByteArray& packString, const QVariant& var);
    QByteArray pack(const QByteArray& packString, const QVariantList& varList);
    int byteInFormatString(const QString& formatString);
    QByteArray convert(const QVariant& var, const QChar& format);
    QByteArray merge(const QByteArray& header, const QByteArray& payload);
    QByteArray populateHeader(int messageID = 120, int srcDevID = 0, int dstDevID = 0, int payload = -1);

signals:
    void newPackage(QVariantList package);
};