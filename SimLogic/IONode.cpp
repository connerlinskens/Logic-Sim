//
// Created by Conner on 2/27/2023.
//

#include "IONode.h"

int IONode::globalIDCounter = 0;

IONode::IONode(enum IONodeType ioNodeType, Vector2 startPos, bool manuallyOverridable)
: _state {false}, _manuallyOverridable{manuallyOverridable}, _position{startPos}, _ioNodeType{ioNodeType}, _id{globalIDCounter++} {
}

bool IONode::State() const {
    return _state;
}

void IONode::ChangeState() {
    _state = !_state;
    for(auto wire : _wires){
        wire->UpdateConnection(_state);
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

Vector2 IONode::AABBExtends() const {
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

void IONode::AddWire(Wire* wire) {
    _wires.push_back(wire);
    wire->UpdateConnection(State());
}

const std::list<Wire*>& IONode::Wires() const{
    return _wires;
}

void IONode::SetPosition(Vector2 position) {
    _position = position;
}

void IONode::RemoveWire(Wire& wire) {
    _wires.remove(&wire);
}

void IONode::ClearWires() {
    _wires.clear();
}
