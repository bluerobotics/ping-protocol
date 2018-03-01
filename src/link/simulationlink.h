#pragma once

#include "abstractlink.h"
#include "protocol.h"

class SimulationLink : public AbstractLink
{
public:
    SimulationLink();
    Protocol* _protocol;
};
