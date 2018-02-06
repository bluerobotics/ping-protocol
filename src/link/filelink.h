#pragma once

#include <QFile>
#include <QTime>

#include "abstractlink.h"

class FileLink : public AbstractLink, QFile
{
public:
    FileLink();
    ~FileLink();

    bool isOpen() final { return isWritable() && isReadable(); };
    bool setConfiguration(const QString& arg) final;
    bool startConnection() final;
    bool finishConnection() final;
    QString errorString() final { return QFile::errorString(); };

private:
    struct Pack {
        QString time;
        QByteArray data;
    };

    QIODevice::OpenModeFlag _openModeFlag;
    QTime _time;

    static const QString _timeFormat;
};