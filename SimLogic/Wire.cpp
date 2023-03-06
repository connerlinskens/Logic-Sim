//
// Created by Conner on 3/2/2023.
//

#include "Wire.h"

Wire::Wire(IONode* nodeA, IONode* nodeB) : _nodeA{nodeA}, _nodeB{nodeB} {
}

void Wire::UpdateConnection(bool state) {
    _nodeB->SetState(state);
    _nodeA->SetState(state);
}

const IONode& Wire::NodeA() const {
    return *_nodeA;
}

const IONode& Wire::NodeB() const {
    return *_nodeB;
}
