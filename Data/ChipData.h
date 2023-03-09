//
// Created by Conner on 3/9/2023.
//

#ifndef LOGIC_SIM_PROGRAMMABLECHIPDATA_H
#define LOGIC_SIM_PROGRAMMABLECHIPDATA_H

#include <string>
#include <vector>
#include "Color.h"
#include "InternalChipData.h"
#include "WireData.h"

struct ChipData{
    std::string name;
    int inputs;
    int outputs;
    Color color;
    std::vector<InternalChipData> internalChips;
    std::vector<WireData> wires;
};

#endif //LOGIC_SIM_PROGRAMMABLECHIPDATA_H
