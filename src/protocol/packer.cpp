#include <QByteArray>
#include <QDebug>
#include <QVariant>

#include "message.h"
#include "packer.h"

Packer::Packer()
{
    qDebug() << "Packer in !";
}

Packer::~Packer()
{
    qDebug() << "Packer out !";
}

bool Packer::validadeData(const QByteArray& data)
{
    qDebug() << __FUNCTION__;
    qDebug() << data;
    if(data.length() < 2) {
        qDebug() << "No data to construct" << data.length();
        return false;
    }
    // Check start byte 1 and 2
    if(data[0] != 'B' && data[1] != 'R') {
        qDebug() << "Wrong start";
        return false;
    }

    unpack(Message::headerPackString(), data);
    return false;
}

QVariantList Packer::decode(QByteArray data)
{
    qDebug() << "decode" << data;
    for(int i(0); i < data.length()-1; i++) {
        data = data.remove(0, i);
        if(!validadeData(data)) {
            qDebug() << "Checksum error !";
            continue;
        }
        qDebug() << "Checksum GOOD !";
    }
    return QVariantList();
}

// TODO: Do this in protocol compilation time
QString Packer::checkPackString(const QString& packString)
{
    if(packString[0] != '<' && packString[0] != '>') {
        qDebug() << "packString need to start with > or < to specify endian.";
        return QString();
    }
    QString formatString = packString;
    for(int i(0); i < formatString.length(); i++) {
        if(formatString[i].isDigit()) {
            int digit = formatString[i].digitValue();
            QString format(formatString[i+1]);
            formatString.replace(formatString.mid(i, 2), format.repeated(digit));
        }
    }
    return formatString;
}

QVariantList Packer::unpack(const QString& packString, QByteArray data)
{
    qDebug() << __FUNCTION__;
    QString formatString = checkPackString(packString);
    if(formatString.isEmpty()) {
        return QVariantList();
    }
    // TODO: work with endian
    formatString = formatString.remove(0, 1);
    QVariantList list;
    for(const auto& format : formatString) {
        qDebug() << "---";
        qDebug() << data << format;
        QVariant var = undo(data, format);
        qDebug() << var;
        qDebug() << data;
        list.append(var);
    }
    qDebug() << "---??" << list;
    return QVariantList();
}

QVariant Packer::undo(QByteArray& data, const QChar& format)
{
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
            break;
    }
}

QByteArray Packer::messagePack(const QVariant& messageID, const QVariant& var)
{
    return pack(Message::packString(messageID), var);
}

QByteArray Packer::messagePack(const QVariant& messageID, const QVariantList& var)
{
    return pack(Message::packString(messageID), var);
}

QByteArray Packer::pack(const QByteArray& packString, const QVariant& var)
{
    QVariantList list;
    list.append(var);
    return pack(packString, list);
}

QByteArray Packer::pack(const QByteArray& packString, const QVariantList& varList)
{
    QString formatString = checkPackString(packString);
    if(formatString.isEmpty()) {
        qDebug() << "formatString cannot be empty !";
        return QByteArray();
    } else if(formatString.length()-1 != varList.length()) {
        qDebug() << "packString and varList differ in size";
        qDebug() << formatString.length()-1 << varList.length();
    }

    QByteArray data;

    // TODO: work with endian
    for(int index(1); index < formatString.length(); index++) {
        QChar format = formatString[index];
        QVariant var = varList[index-1];
        data.append(convert(var, format));
    }

    return data;
}

QByteArray Packer::convert(const QVariant& var, const QChar& format)
{
    uint8_t uint8Value;
    uint16_t uint16Value;
    uint32_t uint32Value;
    uint64_t uint64Value;
    float floatValue;
    double doubleValue;
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

QByteArray Packer::request(const QVariant& messageID, int srcDevID, int dstDevID)
{
    QByteArray headerPack = populateHeader(Message::GeneralMessageID::gen_cmd_request, srcDevID, dstDevID, 2);
    QByteArray payloadPack = messagePack(Message::GeneralMessageID::gen_cmd_request, messageID);
    return merge(headerPack, payloadPack);
}