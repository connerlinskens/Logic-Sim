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

void ProgrammableChip::RemoveChip(Chip* chip, MouseCollisionManager& mouseCollisionManager) {
    if(!chip) { return; }

    auto it = std::find_if(_internalChips.begin(), _internalChips.end(),
                           [&](std::unique_ptr<Chip>& c){return c.get() == chip; });
    if(it != _internalChips.end()){
        for(auto& node : chip->Inputs()){
            // Remove nodes attached to this node
            auto& wires = node->Wires();
            for(auto wire : wires){
                RemoveWire(wire);
                if(wires.empty())
                    break;
            }

            // Remove node
            mouseCollisionManager.RemoveClickable(node.get());
        }
        for(auto& node : chip->Outputs()){
            // Remove wires attached to this node
            auto& wires = node->Wires();
            for(auto wire : wires){
                RemoveWire(wire);
                if(wires.empty())
                    break;
            }

            // Remove node
            mouseCollisionManager.RemoveClickable(node.get());
        }

        // Remove chip
        mouseCollisionManager.RemoveClickable(chip);
        _internalChips.erase(std::remove(_internalChips.begin(), _internalChips.end(), *it), _internalChips.end());
    }
}
