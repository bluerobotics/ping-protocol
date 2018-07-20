#pragma once

#include "abstractlink.h"
#include "parsers/parser.h"

class SimulationLink : public AbstractLink
{
public:
    SimulationLink(QObject* parent = nullptr);

private:
    Parser _parser;
};
