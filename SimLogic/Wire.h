//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_WIRE_H
#define LOGIC_SIM_WIRE_H

#include <vector>
#include "../Data/Vector2.h"
#include "IONode.h"

class IONode;

class Wire {
public:
    Wire(IONode* nodeA, IONode* nodeB, std::vector<Vector2> checkPoints = {});
    void UpdateConnection(bool state);
    [[nodiscard]] IONode& NodeA() const;
    [[nodiscard]] IONode& NodeB() const;

    [[nodiscard]] const std::vector<Vector2>& Checkpoints() const;
private:
    IONode* _nodeA, *_nodeB;
    std::vector<Vector2> _checkpoints;
};


#endif //LOGIC_SIM_WIRE_H
