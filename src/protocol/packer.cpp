#include <QByteArray>
#include <QDebug>
#include <QVariant>

#include "message.h"
#include "packer.h"

Packer::Packer()
{
}

Packer::~Packer()
{
}

bool Packer::validadeData(QByteArray& data, QVariantList& package)
{
    // Check if data have a package
    // Return false if no package available in first n bytes
    // Return true and populate package if data is util

    // Clean package before start
    package.clear();

    QByteArray dataTmp = data;
    if(!dataTmp.length()) {
        qDebug() << "No enough data to construct" << dataTmp.length();
        return false;
    }

    QVariantList header = unpack(Message::headerPackString(), dataTmp);

    // Check start byte 1 and 2
    if(header[0].toChar() != 'B' && header[1].toChar() != 'R') {
        qDebug() << "Wrong start";
        return false;
    }

    uint payloadSize = header[2].toUInt();
    // TODO: This size need to be from message
    uint headerSize = byteInFormatString(Message::headerPackString());
    uint16_t checksum = 0;

    // Check if data have enough information
    uint checksumSize = 2;
    if(payloadSize + headerSize + payloadSize > (uint)dataTmp.length()) {
        return true;
    }

    for(const auto& value : dataTmp.left(headerSize + payloadSize))
        checksum += (uint8_t) value;

    QVariantList dataChecksum = unpack("<H", dataTmp.mid(headerSize + payloadSize, checksumSize));
    if(checksum != dataChecksum[0].toUInt()) {
        qDebug() << "Wrong checksum" << dataTmp.length() << headerSize + payloadSize << dataTmp.mid(headerSize + payloadSize, checksumSize);
        return false;
    }

    QVariantList payload = unpack(Message::packString(header[3]), dataTmp.mid(headerSize, payloadSize));
    package = header + payload;
    data.remove(0, headerSize + payloadSize + checksumSize);

    return true;
}

void Packer::decode(const QByteArray& data)
{
    static QByteArray dataReceived;
    dataReceived.append(data);
    // Decode incoming data
    QVariantList package;
    while(!dataReceived.isEmpty()) {
        if(!validadeData(dataReceived, package)) {
            qDebug() << "no Valida data !";
            dataReceived = dataReceived.remove(0, 1);
            continue;
        }
        if(package.isEmpty()) {
            // Package not done
            // Not enough data to process
            break;
        }
        emit newPackage(package);
    }
}

QVariantList Packer::unpack(const QString& packString, QByteArray data)
{
    QString formatString = packString;
    if(formatString.isEmpty()) {
        qDebug() << "formatString cannot be empty !";
        return QVariantList();
    }

    // TODO: work with endian
    // Remove endian character
    formatString = formatString.remove(0, 1);

    // Transform data in QVariantList package using packString
    QVariantList list;
    for(const auto& format : formatString) {
        QVariant var = unconvert(data, format);
        list.append(var);
    }
    return list;
}

QVariant Packer::unconvert(QByteArray& data, const QChar& format)
{
    // Undo convert function
    // Transform QByteArrays in QVariant with format information
    switch(format.toLatin1()) {
        case('c'):
        case('b'):
        case('B'): {
            auto var = (QChar)data[0];
            data = data.remove(0, 1);
            return var;
        }
        case('h'): {
            const int16_t* var = reinterpret_cast<const int16_t*>(data.left(2).constData());
            data = data.remove(0, 2);
            return var[0];
        }
        case('H'): {
            const uint16_t* var = reinterpret_cast<const uint16_t*>(data.left(2).constData());
            data = data.remove(0, 2);
            return var[0];
        }
        case('i'):
        case('l'): {
            const int32_t* var = reinterpret_cast<const int32_t*>(data.left(4).constData());
            data = data.remove(0, 4);
            return var[0];
        }
        case('I'):
        case('L'): {
            const uint32_t* var = reinterpret_cast<const uint32_t*>(data.left(4).constData());
            data = data.remove(0, 4);
            return var[0];
        }
        case('q'): {
            const long long int* var = reinterpret_cast<const long long int*>(data.left(8).constData());
            data = data.remove(0, 8);
            return var[0];
        }
        case('Q'): {
            const unsigned long long int* var = reinterpret_cast<const unsigned long long int*>(data.left(8).constData());
            data = data.remove(0, 8);
            return var[0];
        }
        case('f'): {
            const float* var =  reinterpret_cast<const float*>(data.left(4).constData());
            data = data.remove(0, 4);
            return var[0];
        }
        case('d'): {
            const double* var =  reinterpret_cast<const double*>(data.left(8).constData());
            data = data.remove(0, 8);
            return var[0];
        }
        default:
            return QVariant();
    }
}

QByteArray Packer::messagePack(int messageID, const QVariant& var)
{
    return pack(Message::packString(messageID), var);
}

QByteArray Packer::messagePack(int messageID, const QVariantList& var)
{
    return pack(Message::packString(messageID), var);
}

QByteArray Packer::pack(const QByteArray& packString, const QVariant& var)
{
    // Transform the QVariantList elements in QByteArray
    QVariantList list;
    list.append(var);
    return pack(packString, list);
}

QByteArray Packer::pack(const QByteArray& packString, const QVariantList& varList)
{
    // Transform the QVariantList elements in QByteArray
    QString formatString = packString;

    // TODO: Work with endian
    formatString.remove(0, 1);
    if(formatString.isEmpty()) {
        qDebug() << "formatString cannot be empty !";
        return QByteArray();
    } else if(formatString.length() != varList.length()) {
        qDebug() << "formatString and varList must have the same size !";
        qDebug() << formatString << varList;
        qDebug() << formatString.length() << varList.length();
    }

    QByteArray data;

    // TODO: work with endian
    for(int index(0); index < formatString.length(); index++) {
        QChar format = formatString[index];
        QVariant var = varList[index];
        data.append(convert(var, format));
    }

    return data;
}

int Packer::byteInFormatString(const QString& formatString)
{
    // Get formatString size in bytes
    int numberOfBytes = 0;
    for(const auto& format : formatString) {
        switch(format.toLatin1()) {
            case('c'):
            case('b'):
            case('B'):
                numberOfBytes += sizeof(uint8_t);
                break;
            case('h'):
            case('H'):
                numberOfBytes += sizeof(uint16_t);
                break;
            case('i'):
            case('I'):
            case('l'):
            case('L'):
                numberOfBytes += sizeof(uint32_t);
                break;
            case('q'):
            case('Q'):
                numberOfBytes += sizeof(uint64_t);
                break;
            case('f'):
                numberOfBytes += sizeof(float);
                break;
            case('d'):
                numberOfBytes += sizeof(double);
                break;
            default:
                break;
        }
    }

    return numberOfBytes;
}

QByteArray Packer::convert(const QVariant& var, const QChar& format)
{
    // Convert QVariant to QByteArray using a format character
    // https://docs.python.org/2/library/struct.html#format-characters
    static uint8_t uint8Value;
    static uint16_t uint16Value;
    static uint32_t uint32Value;
    static uint64_t uint64Value;
    static float floatValue;
    static double doubleValue;
    switch(format.toLatin1()) {
        case('c'):
        case('b'):
        case('B'):
            uint8Value = var.toUInt();
            return QByteArray(reinterpret_cast<const char*>(&uint8Value), sizeof(uint8Value));
        case('h'):
        case('H'):
            uint16Value = var.toUInt();
            return QByteArray(reinterpret_cast<const char*>(&uint16Value), sizeof(uint16Value));
        case('i'):
        case('I'):
        case('l'):
        case('L'):
            uint32Value = var.toUInt();
            return QByteArray(reinterpret_cast<const char*>(&uint32Value), sizeof(uint32Value));
        case('q'):
        case('Q'):
            uint64Value = var.toUInt();
            return QByteArray(reinterpret_cast<const char*>(&uint64Value), sizeof(uint64Value));
        case('f'):
            floatValue = var.toFloat();
            return QByteArray(reinterpret_cast<const char*>(&floatValue), sizeof(floatValue));
        case('d'):
            doubleValue = var.toDouble();
            return QByteArray(reinterpret_cast<const char*>(&doubleValue), sizeof(doubleValue));
        default:
            break;
    }
    return QByteArray();
}

QByteArray Packer::merge(const QByteArray& header, const QByteArray& payload)
{
    // Merge header and payload to finish the package with checksum
    QByteArray data;
    uint16_t checksum = 0;
    for(const auto& value : header + payload)
        checksum += (uint8_t) value;

    data.append(header);
    data.append(payload);
    data.append(pack(Message::checksumPackString(), checksum));
    return data;
}

QByteArray Packer::populateHeader(int messageID, int srcDevID, int dstDevID, int payload)
{
    // This should move to protocol
    QByteArray headerPack = Message::headerPackString();
    QVariantList list;
    list.append('B');
    list.append('R');
    list.append(payload);
    list.append(messageID);
    list.append(srcDevID);
    list.append(dstDevID);

    return pack(headerPack, list);
}

QByteArray Packer::createPack(int messageID, const QVariantList& vars, int srcDevID, int dstDevID)
{
    // It's necessary the payload size to populate header
    QByteArray payloadPack = messagePack(messageID, vars);
    QByteArray headerPack = populateHeader(messageID, srcDevID, dstDevID, payloadPack.length());
    return merge(headerPack, payloadPack);
}

QByteArray Packer::request(int messageID, int srcDevID, int dstDevID)
{
    QVariantList vars;
    vars.append(messageID);
    return createPack(Message::GeneralMessageID::gen_cmd_request, vars, srcDevID, dstDevID);
}