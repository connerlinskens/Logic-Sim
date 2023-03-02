//
// Created by Conner on 3/1/2023.
//

#ifndef LOGIC_SIM_SIMCONTROLMANAGER_H
#define LOGIC_SIM_SIMCONTROLMANAGER_H

#include "../SimLogic/Chip.h"

class SimControlManager {
public:
    void AttachChipToMouse(Chip* chip);
    void ReleaseChip();

    void Update(int mouseX, int mouseY);
private:
    Chip* _selectedChip;
};


#endif //LOGIC_SIM_SIMCONTROLMANAGER_H
