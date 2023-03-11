//
// Created by Conner on 3/1/2023.
//

#ifndef LOGIC_SIM_SIMCONTROLMANAGER_H
#define LOGIC_SIM_SIMCONTROLMANAGER_H

#include <SDL_events.h>
#include "../SimLogic/Chip.h"
#include "../SimLogic/ProgrammableChip.h"
#include "../Data/ChipData.h"
#include <map>

class SimControlManager {
public:
    SimControlManager();

    void AttachChipToMouse(Chip* chip);
    void ReleaseChip();

    void PlaceWire(IONode* node, ProgrammableChip& parentChip);
    void CancelWire();
    [[nodiscard]] bool PlacingWire() const;
    [[nodiscard]] Vector2 SelectedNodeForWire() const;

    void SelectChip(ChipType chipType, const ChipData& chipData);
    [[nodiscard]] bool PlacingChip() const;
    void PlaceChip(ProgrammableChip* parentChip, Vector2 position, const std::map<std::string, ChipData>& chipRecipes, MouseCollisionManager* mouseCollisionManager = nullptr);
    void CancelChip();

    [[nodiscard]] const std::string& NameBuffer() const;
    void ResetNameBuffer();

    void TypeLetter(SDL_Keycode key);
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
    ChipData _selectedChipData;

    std::string _nameBuffer;
};


#endif //LOGIC_SIM_SIMCONTROLMANAGER_H
