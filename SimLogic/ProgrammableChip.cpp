//
// Created by Conner on 3/4/2023.
//

#include "ProgrammableChip.h"
#include <utility>
#include <algorithm>

ProgrammableChip::ProgrammableChip(std::string name, int inputs, int outputs, Color color) : Chip(std::move(name), inputs, outputs, color) {
}

void ProgrammableChip::Execute() {
    for(auto& chip : _internalChips){
        chip->Execute();
    }
}

const std::vector<std::unique_ptr<Chip>>& ProgrammableChip::InternalChips() {
    return _internalChips;
}

Wire& ProgrammableChip::AddInternalWire(IONode* nodeA, IONode* nodeB) {
    auto& wire = _internalWires.emplace_back(std::make_unique<Wire>(nodeA, nodeB));
    nodeA->AddWire(wire.get());
    nodeB->AddWire(wire.get());
    return *wire.get();
}

const std::vector<std::unique_ptr<Wire>>& ProgrammableChip::InternalWires() {
    return _internalWires;
}

void ProgrammableChip::RemoveWire(Wire* wire) {
    if(!wire) { return; }

    auto it = std::find_if(_internalWires.begin(), _internalWires.end(),
                           [&](std::unique_ptr<Wire>& w){return w.get() == wire;});
    if(it != _internalWires.end()){
        wire->NodeA().RemoveWire(*wire);
        wire->NodeB().RemoveWire(*wire);

        _internalWires.erase(std::remove(_internalWires.begin(), _internalWires.end(), *it), _internalWires.end());
    }
}
