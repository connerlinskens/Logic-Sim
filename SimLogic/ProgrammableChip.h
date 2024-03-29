//
// Created by Conner on 3/4/2023.
//

#ifndef LOGIC_SIM_PROGRAMMABLECHIP_H
#define LOGIC_SIM_PROGRAMMABLECHIP_H

#include "Chip.h"
#include <memory>
#include <iterator>

class ProgrammableChip : public Chip {
public:
    ProgrammableChip(std::string name, int inputs, int outputs, Color color = {});
    ~ProgrammableChip() override = default;

    void Execute() override;

    template<typename C, typename... Args>
    C& AddChip(Args... args, Vector2 position = {}){
        C& newChip = static_cast<C&>(*_internalChips.emplace_back(std::make_unique<C>(args...)));
        newChip.SetPosition(position);
        return newChip;
    }

    Chip& AddChip(std::unique_ptr<ProgrammableChip> chip, Vector2 position = {});

    Wire& AddInternalWire(IONode* nodeA, IONode* nodeB, const std::vector<Vector2>& checkPoints = {});

    const std::vector<std::unique_ptr<Chip>>& InternalChips();
    const std::vector<std::unique_ptr<Wire>>& InternalWires();

    void RemoveWire(Wire* wire);
    void RemoveChip(Chip* chip, MouseCollisionManager& mouseCollisionManager);

    void IncrementIONode(bool input, MouseCollisionManager& mouseCollisionManager);
    void DecrementIONode(bool input, MouseCollisionManager& mouseCollisionManager);

private:
    std::vector<std::unique_ptr<Chip>> _internalChips;
    std::vector<std::unique_ptr<Wire>> _internalWires;

    void AddIONode(bool input, MouseCollisionManager& mouseCollisionManager);
};


#endif //LOGIC_SIM_PROGRAMMABLECHIP_H
