#include "message.h"
#include "packer.h"
#include "pingsimulationlink.h"
#include <QtMath>

PingSimulationLink::PingSimulationLink()
{
    _protocol= new Protocol();

    connect(&_randomUpdateTimer, &QTimer::timeout, this, &PingSimulationLink::randomUpdate);
    _randomUpdateTimer.start(50);
}

void PingSimulationLink::randomUpdate()
{
    Packer p;
    QVariantList l;

    static uint counter = 0;
    counter++;
    const float numPoints = 200;
    const float stop1 = numPoints / 2.0 - 10 * qSin(counter / 10.0);
    const float stop2 = 3 * numPoints / 5.0 + 6 * qCos(counter / 5.5);

    uint32_t peak = 50000 * (stop1 + stop2) / 2 / 255;
    uint8_t conf = 400 / (stop2 - stop1);

    l.append(peak);    // * 0-3 distance     u32
    l.append(conf);    // * 4 confidence     u8
    l.append(200);     // * 5-6 pulse_usec   u16
    l.append(counter); // * 7-10 ping_number u32
    l.append(500);     // * 11-14 start_mm   u32
    l.append(50000);   // * 15-18 length_mm  u32
    l.append(4);       // * 19-22 gain_index u32
    l.append(200);     // * 23-24 num_points u16 // Note fixed at 200

    // * 25-n data         u8[200]
    for (int i(0); i < numPoints; i++) {
        float point;
        if (i < stop1) {
            point = 0.1 * (qrand()%256);
        } else if (i < stop2) {
            point = 255 * ((-4.0 / qPow((stop2-stop1), 2.0)) * qPow((i - stop1 - ((stop2-stop1) / 2.0)), 2.0)  + 1.0);
        } else {
            point = 0.45 * (qrand()%256);
        }
        l.append(point);
    }

    emit newData(p.createPack(Message::EchosounderMessageID::es_profile, l, 9, 8));
}
