//
// Created by Conner on 3/2/2023.
//

#include "Wire.h"

Wire::Wire(IONode& inputNode, IONode& outputNode) : _inputNode{inputNode}, _outputNode{outputNode} {
}

void Wire::UpdateConnection(bool state) {
    _inputNode.SetState(state);
}

const IONode& Wire::InputNode() const {
    return _inputNode;
}

const IONode& Wire::OutputNode() const {
    return _outputNode;
}
