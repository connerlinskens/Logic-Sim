//
// Created by Conner on 3/4/2023.
//

#ifndef LOGIC_SIM_CHIPAND_H
#define LOGIC_SIM_CHIPAND_H

#include "../Chip.h"

class ChipAND : public Chip {
public:
    explicit ChipAND(Color color = {200, 200, 50, 255});
    ~ChipAND() override = default;

    void Execute() override;
};


#endif //LOGIC_SIM_CHIPAND_H
