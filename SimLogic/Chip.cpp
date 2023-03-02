//
// Created by Conner on 3/2/2023.
//

#include "Chip.h"

Chip::Chip(std::string name, int inputs, int outputs) : _name{std::move(name)}, _position{Vector2()} {
    int maxNodes = inputs > outputs? inputs : outputs;
    _extends = {100, maxNodes * NodeHeightStep};

    if(inputs > 0){
        int heightStepInputs = _extends.y / inputs;
        for(int i = 0; i < inputs; i++){
            auto node = _inputs.emplace_back(IONodeType::INPUT, Vector2{_position.x - (_extends.x / 2), (_position.y + (heightStepInputs * i) + (heightStepInputs / 2)) - (_extends.y / 2)});
        }
    }

    if(outputs > 0){
        int heightStepOutputs = _extends.y / outputs;
        for(int o = 0; o < outputs; o++){
            auto node = _outputs.emplace_back(IONodeType::OUTPUT, Vector2{_position.x + (_extends.x / 2), (_position.y + (heightStepOutputs * o) + (heightStepOutputs / 2)) - (_extends.y / 2)});
        }
    }
}

void Chip::SetPosition(const Vector2& position) {
    // Translate IONodes with chip
    Vector2 translation = position - _position;
    for(auto& node : _inputs)
        node.Translate(translation);
    for(auto& node : _outputs)
        node.Translate(translation);

    _position = position;
}

void Chip::RegisterToCollisionManager(MouseCollisionManager &mouseCollisionManager) {
    // Add all nodes to manager
    for(auto& node : _inputs)
        mouseCollisionManager.AddClickable(&node);
    for(auto& node : _outputs)
        mouseCollisionManager.AddClickable(&node);

    // Add this chip to manager
    mouseCollisionManager.AddClickable(this);
}

