//
// Created by Conner on 3/4/2023.
//

#ifndef LOGIC_SIM_CHIPNOT_H
#define LOGIC_SIM_CHIPNOT_H

#include "../Chip.h"

class ChipNOT : public Chip {
public:
    explicit ChipNOT(Color color = {125, 200, 50, 255});
    ~ChipNOT() override = default;

    void Execute() override;
};


#endif //LOGIC_SIM_CHIPNOT_H
