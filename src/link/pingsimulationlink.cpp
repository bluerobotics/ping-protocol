#include <QtMath>

#include "pingsimulationlink.h"
#include "pingmessage/pingmessage_all.h"

PingSimulationLink::PingSimulationLink()
{
    connect(&_randomUpdateTimer, &QTimer::timeout, this, &PingSimulationLink::randomUpdate);
    _randomUpdateTimer.start(50);
}

void PingSimulationLink::randomUpdate()
{
    static uint counter = 0;
    counter++;
    static const float numPoints = 200;
    const float stop1 = numPoints / 2.0 - 10 * qSin(counter / 10.0);
    const float stop2 = 3 * numPoints / 5.0 + 6 * qCos(counter / 5.5);

    static const float peakMult = 50000.0f / 2 / 255;
    uint32_t peak = peakMult * (stop1 + stop2);
    uint8_t conf = 400 / (stop2 - stop1);

    static ping_msg_es_profile profile;

    profile.set_distance(peak);
    profile.set_confidence(conf);
    profile.set_pulse_usec(200);
    profile.set_ping_number(counter);
    profile.set_start_mm(500);
    profile.set_length_mm(50000);
    profile.set_gain_index(4);
    profile.set_num_points(200);

    for (int i = 0; i < numPoints; i++) {
        float point;
        if (i < stop1) {
            point = 0.1 * (qrand()%256);
        } else if (i < stop2) {
            point = 255 * ((-4.0 / qPow((stop2-stop1), 2.0)) * qPow((i - stop1 - ((stop2-stop1) / 2.0)), 2.0)  + 1.0);
        } else {
            point = 0.45 * (qrand()%256);
        }
        profile.set_data_at(i, point);
    }

    profile.updateChecksum();
    emit newData(QByteArray(reinterpret_cast<const char*>(profile.msgData.data()), profile.msgData.size()));
}
