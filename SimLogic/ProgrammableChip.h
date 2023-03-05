//
// Created by Conner on 3/4/2023.
//

#ifndef LOGIC_SIM_PROGRAMMABLECHIP_H
#define LOGIC_SIM_PROGRAMMABLECHIP_H

#include "Chip.h"
#include <memory>

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

    const std::vector<std::unique_ptr<Chip>>& InternalChips();
private:
    std::vector<std::unique_ptr<Chip>> _internalChips;
};


#endif //LOGIC_SIM_PROGRAMMABLECHIP_H
