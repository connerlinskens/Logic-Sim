//
// Created by Conner on 2/27/2023.
//

#include "IONode.h"

IONode::IONode(Vector2 startPos, bool manuallyOverridable) : _state {false}, _manuallyOverridable{manuallyOverridable}, _position{startPos} {
}

bool IONode::State() const {
    return _state;
}

void IONode::ChangeState() {
    _state = !_state;
}

bool IONode::IsManuallyOverridable() const {
    return _manuallyOverridable;
}

void IONode::SetManuallyOverridableState(bool state) {
    _manuallyOverridable = state;
}

Vector2 IONode::AABBPosition() {
    return {_position.x - (NodeSize/2), _position.y - (NodeSize/2)};
}

Vector2 IONode::AABBExtends() {
    return {NodeSize,NodeSize};
}

int IONode::ClickLayer() const {
    return 3;
}

void IONode::Clicked() {
    if(IsManuallyOverridable()){
        ChangeState();
    }
}

void IONode::Translate(Vector2 translation) {
    _position += translation;
}

Vector2 IONode::Position() {
    return _position;
}
