//
// Created by Conner on 3/2/2023.
//

#include "Chip.h"
#include <iostream>

#define DefaultChipSize 100
#define ChipCharacterWidthStep 20

Chip::Chip(std::string name, int inputs, int outputs, Color color) : _name{std::move(name)}, _position{Vector2()} {
    int maxNodes = inputs > outputs? inputs : outputs;
    _extends = {_name.size() < 5? DefaultChipSize : DefaultChipSize + (static_cast<int>(_name.size() - 4) * ChipCharacterWidthStep), maxNodes * NodeHeightStep};
    _color = color;

    if(inputs > 0){
        for(int i = 0; i < inputs; i++){
            _inputs.emplace_back(std::make_unique<IONode>(IONodeType::INPUT));
        }
    }

    if(outputs > 0){
        for(int o = 0; o < outputs; o++){
            _outputs.emplace_back(std::make_unique<IONode>(IONodeType::OUTPUT));
        }
    }

    RepositionIONodes();
}

void Chip::SetPosition(const Vector2& position) {
    // Translate IONodes with chip
    Vector2 translation = position - _position;
    for(auto& node : _inputs)
        node->Translate(translation);
    for(auto& node : _outputs)
        node->Translate(translation);

    _position = position;
}

void Chip::RegisterToCollisionManager(MouseCollisionManager &mouseCollisionManager, bool RegisterChip, bool RegisterNodes) {
    if(RegisterNodes){
        // Add all nodes to manager
        for(auto& node : _inputs)
            mouseCollisionManager.AddClickable(node.get());
        for(auto& node : _outputs)
            mouseCollisionManager.AddClickable(node.get());
    }

    if(RegisterChip){
        // Add this chip to manager
        mouseCollisionManager.AddClickable(this);
    }
}

const std::vector<std::unique_ptr<IONode>>& Chip::Inputs() const {
    return _inputs;
}

const std::vector<std::unique_ptr<IONode>>& Chip::Outputs() const {
    return _outputs;
}

void Chip::RepositionIONodes() {
    if(!_inputs.empty()){
        int inputs = static_cast<int>(_inputs.size());
        int heightStepInputs = _extends.y / inputs;
        for(int i = 0; i < inputs; i++){
            _inputs[i]->SetPosition({_position.x - (_extends.x / 2), (_position.y + (heightStepInputs * i) + (heightStepInputs / 2)) - (_extends.y / 2)});
        }
    }

    if(!_outputs.empty()){
        int outputs = static_cast<int>(_outputs.size());
        int heightStepOutputs = _extends.y / outputs;
        for(int o = 0; o < outputs; o++){
            _outputs[o]->SetPosition({_position.x + (_extends.x / 2), (_position.y + (heightStepOutputs * o) + (heightStepOutputs / 2)) - (_extends.y / 2)});
        }
    }
}

void Chip::RepositionIONodesForInternalView(Vector2 windowSize) {
    if(!_inputs.empty()){
        int inputs = static_cast<int>(_inputs.size());
        int heightStepInputs = (windowSize.y-125) / inputs;
        for(int i = 0; i < inputs; i++){
            _inputs[i]->SetPosition({25, 63 + (heightStepInputs * i) + (heightStepInputs / 2)});
        }
    }

    if(!_outputs.empty()){
        int outputs = static_cast<int>(_outputs.size());
        int heightStepOutputs = (windowSize.y-125) / outputs;
        for(int o = 0; o < outputs; o++){
            _outputs[o]->SetPosition({windowSize.x - 25, 63 + (heightStepOutputs * o) + (heightStepOutputs / 2)});
        }
    }
}

void Chip::RemoveIONode(IONode* node, bool input) {
    if(!node) { return; }

    auto& selectedNodes = input? _inputs : _outputs;
    auto it = std::find_if(selectedNodes.begin(), selectedNodes.end(),
                           [&](std::unique_ptr<IONode>& n){return n.get() == node;});
    if(it != selectedNodes.end()){
        selectedNodes.erase(std::remove(selectedNodes.begin(), selectedNodes.end(), *it), selectedNodes.end());
    }
}

const std::string &Chip::Name() const {
    return _name;
}

void Chip::SetName(const std::string &name) {
    _name = name;
}

Color Chip::GetColor() const {
    return _color;
}

void Chip::SetColor(Color color){
    _color = color;
}

void Chip::ResizeChipToFitName() {
    _extends.x = _name.size() < 5? DefaultChipSize : DefaultChipSize + (static_cast<int>(_name.size() - 4) * ChipCharacterWidthStep);
}

void Chip::ResizeChipToFitNodes() {
    int maxNodes = _inputs.size() > _outputs.size()? static_cast<int>(_inputs.size()) : static_cast<int>(_outputs.size());
    _extends.y = maxNodes <= 0? NodeHeightStep : maxNodes * NodeHeightStep;
}
