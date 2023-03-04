//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_LOGICBLOCK_H
#define LOGIC_SIM_LOGICBLOCK_H

#include <utility>
#include <vector>
#include <string>
#include "IONode.h"
#include "../Data/Vector2.h"
#include "../Data/ChipDrawData.h"
#include "../Data/IClickable.h"
#include "../SimManagers/MouseCollisionManager.h"

#define NodeHeightStep 30

class Chip : public IClickable{
public:
    Chip(std::string name, int inputs, int outputs, Color color = {});

    virtual ~Chip() = default;
    virtual void Execute() = 0;

    void SetPosition(const Vector2& position);
    Vector2 Position() { return _position; }

    void RegisterToCollisionManager(MouseCollisionManager& mouseCollisionManager);

    ChipDrawData GetChipDrawData(){
        return ChipDrawData{_name, _position, _extends, _inputs, _outputs, _color};
    }

    Vector2 AABBPosition() override{
        Vector2 extends = AABBExtends();
        return {Position().x - extends.x/2, Position().y - extends.y/2};

    }
    Vector2 AABBExtends() override { return _extends; }
    [[nodiscard]] int ClickLayer() const override { return 1; }
protected:
    std::vector<IONode> _inputs;
    std::vector<IONode> _outputs;

    std::string _name;
    Vector2 _position;
    Vector2 _extends;

    Color _color;
};


#endif //LOGIC_SIM_LOGICBLOCK_H
