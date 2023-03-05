//
// Created by Conner on 3/2/2023.
//

#include "Chip.h"

Chip::Chip(std::string name, int inputs, int outputs, Color color) : _name{std::move(name)}, _position{Vector2()} {
    int maxNodes = inputs > outputs? inputs : outputs;
    _extends = {100, maxNodes * NodeHeightStep};
    _color = color;

    if(inputs > 0){
        for(int i = 0; i < inputs; i++){
            auto node = _inputs.emplace_back(IONodeType::INPUT);
        }
    }

    if(outputs > 0){
        for(int o = 0; o < outputs; o++){
            auto node = _outputs.emplace_back(IONodeType::OUTPUT);
        }
    }

    RepositionIONodes();
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

void Chip::RegisterToCollisionManager(MouseCollisionManager &mouseCollisionManager, bool RegisterChip, bool RegisterNodes) {
    if(RegisterNodes){
        // Add all nodes to manager
        for(auto& node : _inputs)
            mouseCollisionManager.AddClickable(&node);
        for(auto& node : _outputs)
            mouseCollisionManager.AddClickable(&node);
    }

    if(RegisterChip){
        // Add this chip to manager
        mouseCollisionManager.AddClickable(this);
    }
}

const std::vector<IONode> &Chip::Inputs() const {
    return _inputs;
}

const std::vector<IONode> &Chip::Outputs() const {
    return _outputs;
}

void Chip::RepositionIONodes() {
    int inputs = static_cast<int>(_inputs.size());
    int heightStepInputs = _extends.y / inputs;
    for(int i = 0; i < inputs; i++){
        _inputs[i].SetPosition({_position.x - (_extends.x / 2), (_position.y + (heightStepInputs * i) + (heightStepInputs / 2)) - (_extends.y / 2)});
    }

    int outputs = static_cast<int>(_outputs.size());
    int heightStepOutputs = _extends.y / outputs;
    for(int o = 0; o < outputs; o++){
        _outputs[o].SetPosition({_position.x + (_extends.x / 2), (_position.y + (heightStepOutputs * o) + (heightStepOutputs / 2)) - (_extends.y / 2)});
    }
}

void Chip::RepositionIONodesForInternalView(Vector2 windowSize) {
    int inputs = static_cast<int>(_inputs.size());
    int heightStepInputs = windowSize.y / inputs;
    for(int i = 0; i < inputs; i++){
        _inputs[i].SetPosition({25, (heightStepInputs * i) + (heightStepInputs / 2)});
    }

    int outputs = static_cast<int>(_outputs.size());
    int heightStepOutputs = windowSize.y / outputs;
    for(int o = 0; o < outputs; o++){
        _outputs[o].SetPosition({windowSize.x - 25, (heightStepOutputs * o) + (heightStepOutputs / 2)});
    }
}
