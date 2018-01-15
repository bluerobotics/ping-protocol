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
    void sendData(const QByteArray& data);

    // General info
    void deviceType(const QString& devType);
    void deviceModel(const QString& devModel);
    void firmwareVersion(const QString& fwVersion);
    void deviceID(int ID);
    void newData(bool dataAvailable);

    // Echosounder info
    void EchosounderDistance(int mm);
    void EchosounderConfidence(float perc);
    void EchosounderPulseUs(int pulse);
    void EchosounderPingNumber(int pingNumber);
    void EchosounderStart(int mm);
    void EchosounderLength(int mm);
    void EchosounderGain(int gain);
    void EchosounderNumberOfPoints(int numberOfPoints);
    void EchosounderPoints(QList<int> points);
    void EchosounderAuto(bool mode);
    void EchosounderRate(int rate);

    // Mechanical Scanning Sonar info
    void MSSangle(float angle);
    void MSSPulseUs(int pulse);
    void MSSRange(int mm);
    void MSSGain(int gain);
    void MSSNumberOfPoints(int numberOfPoints);
    void MSSPoints(QList<int> points);
    void MSSAuto(bool mode);
    void MSStrainAngle(float angle);
    void MSSsectorWidth(float angle);
    void MSSstepSize(int stepSize);
    void MSSLength(int mm);
    void MSSsampleSize(int size);
};