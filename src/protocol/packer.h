#pragma once

class Packer
{
public:
    Packer();
    ~Packer();

    QString checkPackString(const QString& packString);
    QByteArray pack(const QByteArray& packString, const QVariant& var);
    QByteArray pack(const QByteArray& packString, const QVariantList& varList);
    QByteArray convert(const QVariant& var, const QChar& format);
    QByteArray populateHeader(int messageID = 120, int srcDevID = 0, int dstDevID = 0, int payload = -1);
    QByteArray request(const QVariant& messageID, int srcDevID = 0, int dstDevID = 0);
    QByteArray merge(const QByteArray& header, const QByteArray& payload);
};