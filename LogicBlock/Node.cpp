//
// Created by Conner on 2/27/2023.
//

#include "Node.h"

bool Node::State() const {
    return state;
}

void Node::ChangeState() {
    state = !state;
}
