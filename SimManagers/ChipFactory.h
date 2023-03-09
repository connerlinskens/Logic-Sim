//
// Created by Conner on 3/9/2023.
//

#ifndef LOGIC_SIM_CHIPFACTORY_H
#define LOGIC_SIM_CHIPFACTORY_H

#include <string>
#include "../SimLogic/ProgrammableChip.h"
#include "../Data/ChipData.h"

#define ChipSaveDir "SavedChips/"

class ChipFactory {
public:
    static ChipData PackageChip(ProgrammableChip& chip);
    static std::unique_ptr<ProgrammableChip> FabricateChip(const ChipData& chipData);
private:
    static void SaveChipData(const ChipData& chipData);
};


#endif //LOGIC_SIM_CHIPFACTORY_H
