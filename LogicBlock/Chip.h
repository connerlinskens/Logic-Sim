//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_LOGICBLOCK_H
#define LOGIC_SIM_LOGICBLOCK_H

#include <vector>
#include "Node.h"

class LogicBlock {
public:
    virtual ~LogicBlock() = default;
    virtual void Execute() = 0;
protected:
    std::vector<Node> inputs;
    std::vector<Node> outputs;
};


#endif //LOGIC_SIM_LOGICBLOCK_H
