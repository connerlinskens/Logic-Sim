//
// Created by Conner on 3/4/2023.
//

#include "ProgrammableChip.h"
#include "BasicChips/ChipAND.h"
#include "BasicChips/ChipNOT.h"
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

Chip& ProgrammableChip::AddChip(const Chip& chip) {

}
