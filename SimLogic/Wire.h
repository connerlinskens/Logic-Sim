//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_WIRE_H
#define LOGIC_SIM_WIRE_H

#include "IONode.h"

class IONode;

class Wire {
public:
    Wire(IONode* nodeA, IONode* nodeB);
    void UpdateConnection(bool state);
    [[nodiscard]] const IONode& NodeA() const;
    [[nodiscard]] const IONode& NodeB() const;
private:
    IONode* _nodeA, *_nodeB;
};


#endif //LOGIC_SIM_WIRE_H
