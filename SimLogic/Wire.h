//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_WIRE_H
#define LOGIC_SIM_WIRE_H

#include "IONode.h"

class IONode;

class Wire {
public:
    Wire(IONode& inputNode, IONode& outputNode);
    void UpdateConnection(bool state);
    [[nodiscard]] const IONode& InputNode() const;
    [[nodiscard]] const IONode& OutputNode() const;
private:
    // TODO change these variable names and probably change how wires work with it
    IONode& _inputNode, &_outputNode;
};


#endif //LOGIC_SIM_WIRE_H
