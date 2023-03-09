//
// Created by Conner on 3/9/2023.
//

#ifndef LOGIC_SIM_INTERNALCHIPDATA_H
#define LOGIC_SIM_INTERNALCHIPDATA_H

#include <string>
#include <vector>
#include "Vector2.h"

struct InternalChipData{
    std::string name;
    Vector2 position;
    std::vector<int> inputIDs;
    std::vector<int> outputIDs;
};

#endif //LOGIC_SIM_INTERNALCHIPDATA_H
