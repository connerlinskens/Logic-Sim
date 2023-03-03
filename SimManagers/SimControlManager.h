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
    [[nodiscard]] Vector2 SelectedNodeForWire() const;

    void Update(int mouseX, int mouseY);
private:
    Chip* _selectedChip;
    Vector2 _selectedChipStartOffset;

    bool _placingWire;
    IONode* _tempIONodeWire;
};


#endif //LOGIC_SIM_SIMCONTROLMANAGER_H
