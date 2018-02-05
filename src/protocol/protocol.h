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

    void request(int messageID);
    void handleData(const QByteArray& data);

    // General requests
    Q_INVOKABLE void requestVersion();
    Q_INVOKABLE void requestDeviceID();
    Q_INVOKABLE void requestNewData();

    // Echosounder requests
    Q_INVOKABLE void requestEchosounderDistanceSimple();
    Q_INVOKABLE void requestEchosounderDistance();
    Q_INVOKABLE void requestEchosounderProfile();
    Q_INVOKABLE void requestEchosounderRange();
    Q_INVOKABLE void requestEchosounderMode();
    Q_INVOKABLE void requestEchosounderRate();
    Q_INVOKABLE void requestEchosounderGain();
    Q_INVOKABLE void requestEchosounderPulse();

    // Echosounder setters
    Q_INVOKABLE void setEchosounderGain(int gain);
    Q_INVOKABLE void setEchosounderAuto(bool mode);

    // Mechanical Scanning Sonar requests
    Q_INVOKABLE void requestMSSAngleProfilea();
    Q_INVOKABLE void requestMSSRange();
    Q_INVOKABLE void requestMSSMode();
    Q_INVOKABLE void requestMSSGain();

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
    void echosounderPoints(const QList<double>& points);
    void echosounderAuto(bool mode);
    void echosounderRate(int rate);

    // Mechanical Scanning Sonar info
    void mssAngle(float angle);
    void mssPulseUs(int pulse);
    void mssRange(int mm);
    void mssGain(int gain);
    void mssNumberOfPoints(int numberOfPoints);
    void mssPoints(const QList<int>& points);
    void mssAuto(bool mode);
    void msstrainAngle(float angle);
    void msssectorWidth(float angle);
    void mssstepSize(int stepSize);
    void mssLength(int mm);
    void msssampleSize(int size);

    // To Log
    void emitRawMessages(const QByteArray& package);

private:
    void emitMessages(const QVariantList& package);
};