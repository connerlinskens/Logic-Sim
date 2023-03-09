//
// Created by Conner on 3/9/2023.
//

#include "ChipFactory.h"

std::string ChipFactory::PackageChip(ProgrammableChip &chip) {
    std::string chipData {};
    return chipData;
}

std::unique_ptr<ProgrammableChip> ChipFactory::FabricateChip(const std::string &chipData) {
    auto chip {std::make_unique<ProgrammableChip>("TEST", 1, 1)};
    return std::move(chip);
}
