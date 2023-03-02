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
#include "../Sim/MouseCollisionManager.h"

#define NodeHeightStep 30

class Chip : public IClickable{
public:
    Chip(std::string name, int inputs, int outputs) : _name{std::move(name)}, _position{Vector2()} {
        int maxNodes = inputs > outputs? inputs : outputs;
        _extends = {100, maxNodes * NodeHeightStep};

        int heightStepInputs = _extends.y / inputs;
        for(int i = 0; i < inputs; i++){
            auto node = _inputs.emplace_back(Vector2{_position.x - (_extends.x / 2), (_position.y + (heightStepInputs * i) + (heightStepInputs / 2)) - (_extends.y / 2)});
        }
        int heightStepOutputs = _extends.y / outputs;
        for(int o = 0; o < outputs; o++){
            auto node = _outputs.emplace_back(Vector2{_position.x + (_extends.x / 2), (_position.y + (heightStepOutputs * o) + (heightStepOutputs / 2)) - (_extends.y / 2)});
        }
    };

    virtual ~Chip() = default;
    virtual void Execute() {};

    void SetPosition(const Vector2 position){
        // Translate IONodes with chip
        Vector2 translation = position - _position;
        for(auto& node : _inputs)
            node.Translate(translation);
        for(auto& node : _outputs)
            node.Translate(translation);

        _position = position;
    }
    Vector2 Position() { return _position; }

    ChipDrawData GetChipDrawData(){
        return ChipDrawData{_name, _position, _extends, _inputs, _outputs};
    }

    Vector2 AABBPosition() override{
        Vector2 extends = AABBExtends();
        return {Position().x - extends.x/2, Position().y - extends.y/2};

    }
    Vector2 AABBExtends() override {
        return _extends;
    }

    void RegisterToCollisionManager(MouseCollisionManager& mouseCollisionManager){
        // Add all nodes to manager
        for(auto& node : _inputs)
            mouseCollisionManager.AddClickable(&node);
        for(auto& node : _outputs)
            mouseCollisionManager.AddClickable(&node);

        // Add this chip to manager
        mouseCollisionManager.AddClickable(this);
    }

protected:
    std::vector<IONode> _inputs;
    std::vector<IONode> _outputs;

    std::string _name;
    Vector2 _position;
    Vector2 _extends;
};


#endif //LOGIC_SIM_LOGICBLOCK_H
