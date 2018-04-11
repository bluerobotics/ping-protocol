#include <QDateTime>
#include <QDebug>

#include "sensor.h"

#include "pingmessage/pingmessage.h"
#include "pingmessage/pingmessage_gen.h"
#include "pingmessage/pingmessage_es.h"

Sensor::Sensor() :
     _linkIn(new Link(AbstractLink::LinkType::Serial, "Default"))
    ,_linkOut(nullptr)
    ,_parser(nullptr)
{

}

// TODO rework this after sublasses and parser rework
void Sensor::connectLink(const QString& connString)
{
    if(link()->isOpen()) {
        link()->finishConnection();
    }

    qDebug() << "connecting to" << connString;
    QStringList confList = connString.split(':');
    if(confList.length() != 3) {
        qDebug() << "wrong size !";
        return;
    }
    if(confList[0].toInt() <= 0 || confList[0].toInt() > 5) {
        qDebug() << "wrong arg !";
        return;
    }
    if(_linkIn) {
        delete _linkIn;
    }
    _linkIn = new Link((AbstractLink::LinkType)confList[0].toInt(), "Default");
    confList.removeFirst();
    QString conf = confList.join(':');

    link()->setConfiguration(conf);
    link()->startConnection();

    if(!link()->isOpen()) {
        qDebug() << "Connection fail !" << connString << link()->errorString();;
        emit connectionClose();
        return;
    }

    emit linkUpdate();

    if (_parser) {
        connect(link(), &AbstractLink::newData, _parser, &Parser::parseBuffer);

    }

    emit connectionOpen();

    // Disable log if playing one
    if(link()->type() == AbstractLink::LinkType::File) {
        if(!_linkOut) {
            return;
        }
        if(linkLog()->isOpen()) {
            linkLog()->finishConnection();
            _linkOut->deleteLater();
        }
    } else { // Start log, if not playing one
        QString fileName = QStringLiteral("%1.%2").arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-hhmmsszzz")), "bin");
        QString config = QStringLiteral("%1:%2:%3").arg(QString::number(1), fileName, "w");
        connectLinkLog(config);
    }
}

void Sensor::connectLinkLog(const QString& connString)
{
    if(_linkOut) {
        if(!_linkOut->isOpen()) {
            qDebug() << "No connection to log !" << linkLog()->errorString();
            return;
        }
        delete _linkOut;
    }

    QStringList confList = connString.split(':');
    if(confList.length() != 3) {
        qDebug() << "wrong size !" << confList;
        return;
    }
    if(confList[0].toInt() <= 0 || confList[0].toInt() > 5) {
        qDebug() << "wrong arg !" << confList;
        return;
    }

    _linkOut = new Link((AbstractLink::LinkType)confList[0].toInt(), "Log");
    confList.removeFirst();
    QString conf = confList.join(':');

    linkLog()->setConfiguration(conf);
    linkLog()->startConnection();

    if(!linkLog()->isOpen()) {
        qDebug() << "Connection with log fail !" << connString << linkLog()->errorString();
        return;
    }

    emit linkLogUpdate();
}

Sensor::~Sensor()
{
}
