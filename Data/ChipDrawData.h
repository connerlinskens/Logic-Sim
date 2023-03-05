//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_CHIPDRAWDATA_H
#define LOGIC_SIM_CHIPDRAWDATA_H

#include <string>
#include <vector>
#include "../SimLogic/IONode.h"
#include "Vector2.h"
#include "Color.h"

struct ChipDrawData{
    std::string name;
    Vector2 position;
    Vector2 extends;
    Color drawColor = Color();
};

#endif //LOGIC_SIM_CHIPDRAWDATA_H
