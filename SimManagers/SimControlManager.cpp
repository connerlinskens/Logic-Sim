//
// Created by Conner on 3/1/2023.
//

#include "SimControlManager.h"

void SimControlManager::AttachChipToMouse(Chip *chip) {
    _selectedChip = chip;
    // TODO add start offset
}

void SimControlManager::ReleaseChip() {
    _selectedChip = nullptr;
}

void SimControlManager::Update(int mouseX, int mouseY) {
    // If a chip is selected, move it to mouse position
    if(_selectedChip){
        _selectedChip->SetPosition({mouseX, mouseY});
    }
}
