//
// Created by Conner on 2/27/2023.
//

#include "IONode.h"

IONode::IONode(enum IONodeType ioNodeType, Vector2 startPos, bool manuallyOverridable) : _state {false}, _manuallyOverridable{manuallyOverridable}, _position{startPos}, _ioNodeType{ioNodeType} {
}

bool IONode::State() const {
    return _state;
}

void IONode::ChangeState() {
    _state = !_state;
    for(auto& wire : _wires){
        wire.UpdateConnection(_state);
    }
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

Vector2 IONode::Position() const{
    return _position;
}

IONodeType IONode::IONodeType() const {
    return _ioNodeType;
}

void IONode::SetState(bool newState) {
    if(newState != State())
        ChangeState();
}

void IONode::AddWire(IONode* node) {
//    if(IONodeType() == IONodeType::OUTPUT){
        auto& wire = _wires.emplace_back(*node, *this);
        wire.UpdateConnection(State());
//    }
}

const std::vector<Wire>& IONode::Wires() const{
    return _wires;
}
