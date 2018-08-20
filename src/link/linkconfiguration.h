#pragma once

#include <QDebug>
#include <QMap>
#include <QString>
#include <QStringList>

#include "abstractlinknamespace.h"

class LinkConfiguration : QObject
{
    Q_OBJECT

public:
    struct LinkConf {
        QStringList args;
        QString name;
        LinkType type = LinkType::None;
    };

    enum Error {
        MissingConfiguration, // This can be used in future for warnings and not real errors
        NoErrors = 0,
        NoType,
        InvalidType,
        NoArgs,
        InvalidArgsNumber,
        ArgsAreEmpty,
    };

    static const QMap<Error, QString> errorMap;

    LinkConfiguration(LinkType linkType = LinkType::None, QStringList args = QStringList(), QString name = QString())
        : _linkConf{args, name, linkType} {};
    LinkConfiguration(LinkConf& confLinkStructure)
        : _linkConf{confLinkStructure} {};
    ~LinkConfiguration() = default;

    const QStringList* args() const { return &_linkConf.args; };
    QStringList argsAsConst() const { return _linkConf.args; };

    QString name() const { return _linkConf.name; };
    void setName(QString name) { _linkConf.name = name; };

    LinkType type() const { return _linkConf.type; };
    void setType(LinkType type) { _linkConf.type = type; };

    const QString createConfString() const { return _linkConf.args.join(":"); };
    const QStringList createConfStringList() const { return _linkConf.args; };

    const QString createFullConfString() const;
    const QStringList createFullConfStringList() const;

    bool isValid() const { return error() <= NoErrors; }

    Error error() const;

    static QString errorToString(Error error) { return errorMap[error]; }

    //Todo: move to `operator QString()`
    QString toString() const;

private:
    LinkConf _linkConf;
};