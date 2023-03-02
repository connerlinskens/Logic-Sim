//
// Created by Conner on 3/1/2023.
//

#ifndef LOGIC_SIM_SIMCONTROLMANAGER_H
#define LOGIC_SIM_SIMCONTROLMANAGER_H

#include "../SimLogic/Chip.h"

class SimControlManager {
public:
    SimControlManager();

    void AttachChipToMouse(Chip* chip);
    void ReleaseChip();

    void PlaceWire(IONode* node);
    void CancelWire();
    [[nodiscard]] bool PlacingWire() const;

    void Update(int mouseX, int mouseY);
private:
    Chip* _selectedChip;
    bool _placingWire;
    IONode* _tempIONodeWire;
};


#endif //LOGIC_SIM_SIMCONTROLMANAGER_H
