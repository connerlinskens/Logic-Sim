//
// Created by Conner on 3/2/2023.
//

#include "Wire.h"

#include <utility>

Wire::Wire(IONode* nodeA, IONode* nodeB, std::vector<Vector2> checkPoints) : _nodeA{nodeA}, _nodeB{nodeB}, _checkpoints{std::move(checkPoints)} {
}

void Wire::UpdateConnection(bool state) {
    _nodeB->SetState(state);
    _nodeA->SetState(state);
}

IONode& Wire::NodeA() const {
    return *_nodeA;
}

IONode& Wire::NodeB() const {
    return *_nodeB;
}

const std::vector<Vector2>& Wire::Checkpoints() const {
    return _checkpoints;
}
