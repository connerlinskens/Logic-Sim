//
// Created by Conner on 3/9/2023.
//

#ifndef LOGIC_SIM_CHIPFACTORY_H
#define LOGIC_SIM_CHIPFACTORY_H

#include <string>
#include "../SimLogic/ProgrammableChip.h"

#define ChipSaveDir "SavedChips"

class ChipFactory {
public:
    static std::string PackageChip(ProgrammableChip& chip);
    static std::unique_ptr<ProgrammableChip> FabricateChip(const std::string& chipData);
};


#endif //LOGIC_SIM_CHIPFACTORY_H
