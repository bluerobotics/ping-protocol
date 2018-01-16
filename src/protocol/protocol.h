#pragma once

#include <QObject>

#include "message.h"
#include "packer.h"

class Protocol : public QObject
{
    Q_OBJECT
public:
    Protocol();
    ~Protocol();

    Message _message;
    Packer* _packer;

    void request(QVariant messageID);
    void handleData(const QByteArray& data);

    // General requests
    void requestVersion();
    void requestDeviceID();
    void requestNewData();

    // Echosounder requests
    void requestEchosounderDistanceSimple();
    void requestEchosounderDistance();
    void requestEchosounderProfile();
    void requestEchosounderRange();
    void requestEchosounderMode();
    void requestEchosounderRate();
    void requestEchosounderGain();
    void requestEchosounderPulse();

    // Mechanical Scanning Sonar requests
    void requestMSSAngleProfilea();
    void requestMSSRange();
    void requestMSSMode();
    void requestMSSGain();

signals:
    void update();
    void sendData(const QByteArray& data);

    // General info
    void deviceType(const QString& devType);
    void deviceModel(const QString& devModel);
    void firmwareVersion(const QString& fwVersion);
    void deviceID(int ID);
    void newData(bool dataAvailable);

    // Echosounder info
    void echosounderDistance(int mm);
    void echosounderConfidence(float perc);
    void echosounderPulseUs(int pulse);
    void echosounderPingNumber(int pingNumber);
    void echosounderStart(int mm);
    void echosounderLength(int mm);
    void echosounderGain(int gain);
    void echosounderNumberOfPoints(int numberOfPoints);
    void echosounderPoints(QList<double> points);
    void echosounderAuto(bool mode);
    void echosounderRate(int rate);

    // Mechanical Scanning Sonar info
    void mssAngle(float angle);
    void mssPulseUs(int pulse);
    void mssRange(int mm);
    void mssGain(int gain);
    void mssNumberOfPoints(int numberOfPoints);
    void mssPoints(QList<int> points);
    void mssAuto(bool mode);
    void msstrainAngle(float angle);
    void msssectorWidth(float angle);
    void mssstepSize(int stepSize);
    void mssLength(int mm);
    void msssampleSize(int size);

private:
    void emitMessages(QVariantList package);
};