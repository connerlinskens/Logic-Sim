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
        wire->UpdateConnection(false);
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
                auto wireIt = std::find_if(_internalWires.begin(), _internalWires.end(),
                                       [&](std::unique_ptr<Wire>& w){return w.get() == wire;});
                if(wireIt != _internalWires.end()){
                    // Update the state of the attached node
                    wireIt->get()->UpdateConnection(false);

                    // Remove reference from other end of the wire
                    if(&wireIt->get()->NodeA() != node.get())
                        wireIt->get()->NodeA().RemoveWire(*wire);
                    else if(&wireIt->get()->NodeB() != node.get())
                        wireIt->get()->NodeB().RemoveWire(*wire);

                    // Remove wire
                    _internalWires.erase(std::remove(_internalWires.begin(), _internalWires.end(), *wireIt), _internalWires.end());
                }
            }

            // Remove node
            mouseCollisionManager.RemoveClickable(node.get());
        }
        for(auto& node : chip->Outputs()){
            // Remove wires attached to this node
            auto& wires = node->Wires();
            for(auto wire : wires){
                auto wireIt = std::find_if(_internalWires.begin(), _internalWires.end(),
                                           [&](std::unique_ptr<Wire>& w){return w.get() == wire;});
                if(wireIt != _internalWires.end()){
                    // Update the state of the attached node
                    wireIt->get()->UpdateConnection(false);

                    // Remove reference from other end of the wire
                    if(&wireIt->get()->NodeA() != node.get())
                        wireIt->get()->NodeA().RemoveWire(*wire);
                    else if(&wireIt->get()->NodeB() != node.get())
                        wireIt->get()->NodeB().RemoveWire(*wire);

                    // Remove wire
                    _internalWires.erase(std::remove(_internalWires.begin(), _internalWires.end(), *wireIt), _internalWires.end());
                }
            }

            // Remove node
            mouseCollisionManager.RemoveClickable(node.get());
        }

        // Remove chip
        mouseCollisionManager.RemoveClickable(chip);
        _internalChips.erase(std::remove(_internalChips.begin(), _internalChips.end(), *it), _internalChips.end());
    }
}

Chip& ProgrammableChip::AddChip(std::unique_ptr<ProgrammableChip> chip, Vector2 position) {
    auto& newChip = _internalChips.emplace_back(std::move(chip));
    newChip->SetPosition(position);
    return *newChip.get();
}

void ProgrammableChip::IncrementIONode(bool input, MouseCollisionManager& mouseCollisionManager) {
    auto& selectedNodes = input? _inputs : _outputs;
    AddIONode(input, mouseCollisionManager);
}

void ProgrammableChip::AddIONode(bool input, MouseCollisionManager& mouseCollisionManager) {
    auto& selectedNodes = input? _inputs : _outputs;
    auto& newNode = selectedNodes.emplace_back(std::make_unique<IONode>(input? IONodeType::INPUT : IONodeType::OUTPUT));
    mouseCollisionManager.AddClickable(newNode.get());
    newNode->SetManuallyOverridableState(true);
}

void ProgrammableChip::DecrementIONode(bool input, MouseCollisionManager& mouseCollisionManager) {
    auto& selectedNodes = input? _inputs : _outputs;
    auto node {selectedNodes.at(input? _inputs.size()-1 : _outputs.size()-1).get()};
    for(auto wire : node->Wires()) {
        auto wireIt = std::find_if(_internalWires.begin(), _internalWires.end(),
                                   [&](std::unique_ptr<Wire> &w) { return w.get() == wire; });
        if (wireIt != _internalWires.end()) {
            // Update the state of the attached node
            wireIt->get()->UpdateConnection(false);

            // Remove reference from other end of the wire
            if (&wireIt->get()->NodeA() != node)
                wireIt->get()->NodeA().RemoveWire(*wire);
            else if (&wireIt->get()->NodeB() != node)
                wireIt->get()->NodeB().RemoveWire(*wire);

            // Remove wire
            _internalWires.erase(std::remove(_internalWires.begin(), _internalWires.end(), *wireIt),
                                 _internalWires.end());
        }
    }
    mouseCollisionManager.RemoveClickable(node);
    RemoveIONode(node, input);
}
