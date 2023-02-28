//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_CHIPDRAWDATA_H
#define LOGIC_SIM_CHIPDRAWDATA_H

#include <string>
#include "Vector2.h"

struct ChipDrawData{
    std::string name;
    Vector2 position;
    int inputs;
    int outputs;
};

#endif //LOGIC_SIM_CHIPDRAWDATA_H
