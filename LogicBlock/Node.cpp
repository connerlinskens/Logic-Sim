//
// Created by Conner on 2/27/2023.
//

#include "Node.h"

Node::Node(bool manuallyOverridable) : _state {false}, _manuallyOverridable{manuallyOverridable} {
}

bool Node::State() const {
    return _state;
}

void Node::ChangeState() {
    _state = !_state;
}

bool Node::IsManuallyOverridable() const {
    return _manuallyOverridable;
}

void Node::SetManuallyOverridableState(bool state) {
    _manuallyOverridable = state;
}


