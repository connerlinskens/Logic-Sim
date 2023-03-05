//
// Created by Conner on 3/1/2023.
//

#include <iostream>
#include "SimControlManager.h"
#include "RenderManager.h"
#include "../SimLogic/BasicChips/ChipAND.h"
#include "../SimLogic/BasicChips/ChipOR.h"
#include "../SimLogic/BasicChips/ChipNOT.h"

SimControlManager::SimControlManager() : _placingWire{false}, _selectedChip{nullptr}, _tempIONodeWire{nullptr}, _placingChip{false}, _selectedChipType{ChipType::PROGRAMMABLE} {
}

void SimControlManager::AttachChipToMouse(Chip *chip) {
    if(_placingWire) { return; }

    _selectedChip = chip;
}

void SimControlManager::ReleaseChip() {
    if(_placingWire || _selectedChip == nullptr) { return; }

    _selectedChip = nullptr;
    _selectedChipStartOffset = {};
}

void SimControlManager::Update(int mouseX, int mouseY) {
    // If a chip is selected, move it to mouse position
    if(_selectedChip){
        if(_selectedChipStartOffset == Vector2{0,0})
            _selectedChipStartOffset = _selectedChip->Position() - Vector2{mouseX, mouseY};
        Vector2 newPosition = {mouseX + _selectedChipStartOffset.x, mouseY + _selectedChipStartOffset.y};
        _selectedChip->SetPosition(newPosition);
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
        _placingWire = false;
        _tempIONodeWire->AddWire(node);
    }
}

void SimControlManager::CancelWire() {
    _tempIONodeWire = nullptr;
    _placingWire = false;
}

bool SimControlManager::PlacingWire() const {
    return _placingWire;
}

Vector2 SimControlManager::SelectedNodeForWire() const {
    if(!_placingWire) return {};

    return _tempIONodeWire->Position();
}

void SimControlManager::SelectChip(ChipType chipType) {
    _placingChip = true;
    _selectedChipType = chipType;
}

void SimControlManager::PlaceChip(ProgrammableChip* parentChip, Vector2 position, const std::string& chipData, MouseCollisionManager* mouseCollisionManager) {
    if(_selectedChipType == ChipType::AND){
        auto& chip = parentChip->AddChip<ChipAND>(position);
        if(mouseCollisionManager)
            chip.RegisterToCollisionManager(*mouseCollisionManager);
    }
    else if(_selectedChipType == ChipType::OR){
        auto& chip = parentChip->AddChip<ChipOR>(position);
        if(mouseCollisionManager)
            chip.RegisterToCollisionManager(*mouseCollisionManager);
    }
    else if(_selectedChipType == ChipType::NOT){
        auto& chip = parentChip->AddChip<ChipNOT>(position);
        if(mouseCollisionManager)
            chip.RegisterToCollisionManager(*mouseCollisionManager);
    }
    else if(_selectedChipType == ChipType::PROGRAMMABLE){
        // TODO implement placing programmable chips
    }
    _placingChip = false;
}

bool SimControlManager::PlacingChip() const {
    return _placingChip;
}

void SimControlManager::CancelChip() {
    _placingChip = false;
}
