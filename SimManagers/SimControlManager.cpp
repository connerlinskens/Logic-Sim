//
// Created by Conner on 3/1/2023.
//

#include <iostream>
#include "SimControlManager.h"

SimControlManager::SimControlManager() : _placingWire{false}, _selectedChip{nullptr}, _tempIONodeWire{nullptr} {
}

void SimControlManager::AttachChipToMouse(Chip *chip) {
    if(_placingWire) { return; }

    _selectedChip = chip;
    // TODO save start offset
}

void SimControlManager::ReleaseChip() {
    if(_placingWire || _selectedChip == nullptr) { return; }

    _selectedChip = nullptr;
}

void SimControlManager::Update(int mouseX, int mouseY) {
    // If a chip is selected, move it to mouse position
    if(_selectedChip){
        // TODO apply start offset
        _selectedChip->SetPosition({mouseX, mouseY});
    }
}

void SimControlManager::PlaceWire(IONode* node) {
    // Start wire placement
    if(!_placingWire){
        _placingWire = true;
        _tempIONodeWire = node;
    }
    // Finish wire placement
    else{
        // Cant connect two inputs or outputs together
        if(node->IONodeType() == _tempIONodeWire->IONodeType()) { return; }

        _placingWire = false;
        // Create Wire in OUTPUT Node
        if(node->IONodeType() == IONodeType::OUTPUT){
            node->AddWire(_tempIONodeWire);
        }
        else if(_tempIONodeWire->IONodeType() == IONodeType::OUTPUT){
            _tempIONodeWire->AddWire(node);
        }
        else{
            std::cerr << "Found IO Node with unknown type in PlaceWire() from SimControlManager" << std::endl;
        }
    }
}

void SimControlManager::CancelWire() {
    _tempIONodeWire = nullptr;
    _placingWire = false;
}

bool SimControlManager::PlacingWire() const {
    return _placingWire;
}
