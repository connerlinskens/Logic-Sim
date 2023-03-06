//
// Created by Conner on 3/4/2023.
//

#include "ProgrammableChip.h"
#include <utility>

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
