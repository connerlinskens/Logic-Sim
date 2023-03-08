//
// Created by Conner on 3/1/2023.
//

#ifndef LOGIC_SIM_SIMCONTROLMANAGER_H
#define LOGIC_SIM_SIMCONTROLMANAGER_H

#include "../SimLogic/Chip.h"
#include "../SimLogic/ProgrammableChip.h"

class SimControlManager {
public:
    SimControlManager();

    void AttachChipToMouse(Chip* chip);
    void ReleaseChip();

    void PlaceWire(IONode* node, ProgrammableChip& parentChip);
    void CancelWire();
    [[nodiscard]] bool PlacingWire() const;
    [[nodiscard]] Vector2 SelectedNodeForWire() const;

    void SelectChip(ChipType chipType);
    [[nodiscard]] bool PlacingChip() const;
    void PlaceChip(ProgrammableChip* parentChip, Vector2 position, const std::string& chipData = {}, MouseCollisionManager* mouseCollisionManager = nullptr);
    void CancelChip();

    void Update(int mouseX, int mouseY);
private:
    void FinishWire(IONode* node, ProgrammableChip& parentChip);
private:
    Chip* _selectedChip;
    Vector2 _selectedChipStartOffset;

    bool _placingWire;
    IONode* _tempIONodeWire;

    bool _placingChip;
    ChipType _selectedChipType;
};


#endif //LOGIC_SIM_SIMCONTROLMANAGER_H
