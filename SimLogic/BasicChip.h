//
// Created by Conner on 3/3/2023.
//

#ifndef LOGIC_SIM_BASICCHIP_H
#define LOGIC_SIM_BASICCHIP_H

#include "Chip.h"
#include <functional>

class BasicChip : public Chip {
public:
    BasicChip(std::string name, int inputs, int outputs, std::function<bool(bool, bool)> logic, Color color = {});
    ~BasicChip() override = default;

    void Execute() override;

private:
    std::function<bool(bool, bool)> _logic;
};


#endif //LOGIC_SIM_BASICCHIP_H
