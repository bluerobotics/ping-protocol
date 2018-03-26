#pragma once

#include "abstractlink.h"
#include "parsers/parser.h"

class SimulationLink : public AbstractLink
{
public:
    SimulationLink();
    Parser _parser;
};
