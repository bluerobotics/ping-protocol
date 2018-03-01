#pragma once

#include <QTimer>
#include "simulationlink.h"

class PingSimulationLink : public SimulationLink
{
public:
    PingSimulationLink();
    void randomUpdate();

    QTimer _randomUpdateTimer;
};
