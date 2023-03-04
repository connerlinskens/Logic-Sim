//
// Created by Conner on 3/4/2023.
//

#ifndef LOGIC_SIM_CHIPOR_H
#define LOGIC_SIM_CHIPOR_H

#include "../Chip.h"

class ChipOR : public Chip {
public:
    explicit ChipOR(Color color = {50, 200, 200, 255});
    ~ChipOR() override = default;

    void Execute() override;
};


#endif //LOGIC_SIM_CHIPOR_H
