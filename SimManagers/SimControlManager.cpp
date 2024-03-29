//
// Created by Conner on 3/1/2023.
//

#include <iostream>
#include "SimControlManager.h"
#include "RenderManager.h"
#include "../SimLogic/BasicChips/ChipAND.h"
#include "../SimLogic/BasicChips/ChipOR.h"
#include "../SimLogic/BasicChips/ChipNOT.h"
#include "ChipFactory.h"

IONode* SimControlManager::_highlightedIONode = nullptr;

SimControlManager::SimControlManager() : _placingWire{false}, _selectedChip{nullptr}, _tempIONodeWire{nullptr}, _placingChip{false}, _selectedChipType{ChipType::PROGRAMMABLE}, _tempWireCheckPoints{{}} {
}

void SimControlManager::AttachChipToMouse(Chip *chip) {
    if(_placingWire) { return; }

    _selectedChip = chip;
}

void SimControlManager::ReleaseChip() {
    if(_placingWire || _selectedChip == nullptr) { return; }

    _selectedChip = nullptr;
    _selectedChipStartOffset = {};
}

void SimControlManager::Update(int mouseX, int mouseY) {
    // If a chip is selected, move it to mouse position
    if(_selectedChip){
        if(_selectedChipStartOffset == Vector2{0,0})
            _selectedChipStartOffset = _selectedChip->Position() - Vector2{mouseX, mouseY};
        Vector2 newPosition = {mouseX + _selectedChipStartOffset.x, mouseY + _selectedChipStartOffset.y};
        _selectedChip->SetPosition(newPosition);
    }
}

void SimControlManager::PlaceWire(IONode* node, ProgrammableChip& parentChip) {
    // Return if trying to connect to the same node
    if(_tempIONodeWire == node) {return;}

    // Start wire placement
    if(!_placingWire){
        _placingWire = true;
        _tempIONodeWire = node;
        _tempWireCheckPoints.clear();
    }
    // Finish wire placement
    else if(_tempIONodeWire->IONodeType() != node->IONodeType()){
        FinishWire(node, parentChip);
    }
    else {
        bool parentNodeFound {false};
        for(auto& input : parentChip.Inputs()){
            if(input->ID() == _tempIONodeWire->ID() || input->ID() == node->ID()){
                parentNodeFound = true;
                break;
            }
        }

        if(!parentNodeFound){
            for(auto& output : parentChip.Outputs()){
                if(output->ID() == _tempIONodeWire->ID() || output->ID() == node->ID()){
                    parentNodeFound = true;
                    break;
                }
            }
        }

        if(parentNodeFound){
            if(_tempIONodeWire->IONodeType() == node->IONodeType()){
                FinishWire(node, parentChip);
            }
        }
    }
}

void SimControlManager::CancelWire() {
    _tempIONodeWire = nullptr;
    _placingWire = false;
    _tempWireCheckPoints.clear();
}

bool SimControlManager::PlacingWire() const {
    return _placingWire;
}

Vector2 SimControlManager::SelectedNodeForWire() const {
    if(!_placingWire) return {};
    return _tempIONodeWire->Position();
}

void SimControlManager::SelectChip(ChipType chipType, const ChipData& chipData) {
    _placingChip = true;
    _selectedChipType = chipType;
    _selectedChipData = chipData;
}

void SimControlManager::PlaceChip(ProgrammableChip* parentChip, Vector2 position, const std::map<std::string, ChipData>& chipRecipes, MouseCollisionManager* mouseCollisionManager) {
    if(_selectedChipType == ChipType::AND){
        auto& chip = parentChip->AddChip<ChipAND>(position);
        if(mouseCollisionManager)
            chip.RegisterToCollisionManager(*mouseCollisionManager);
    }
    else if(_selectedChipType == ChipType::OR){
        auto& chip = parentChip->AddChip<ChipOR>(position);
        if(mouseCollisionManager)
            chip.RegisterToCollisionManager(*mouseCollisionManager);
    }
    else if(_selectedChipType == ChipType::NOT){
        auto& chip = parentChip->AddChip<ChipNOT>(position);
        if(mouseCollisionManager)
            chip.RegisterToCollisionManager(*mouseCollisionManager);
    }
    else if(_selectedChipType == ChipType::PROGRAMMABLE){
        auto& chip = parentChip->AddChip(std::move(ChipFactory::FabricateChip(_selectedChipData, chipRecipes)), position);
        if(mouseCollisionManager)
            chip.RegisterToCollisionManager(*mouseCollisionManager);
    }
    _placingChip = false;
}

bool SimControlManager::PlacingChip() const {
    return _placingChip;
}

void SimControlManager::CancelChip() {
    _placingChip = false;
}

void SimControlManager::FinishWire(IONode* node, ProgrammableChip& parentChip) {
    _placingWire = false;
    parentChip.AddInternalWire(_tempIONodeWire, node, _tempWireCheckPoints);
    _tempIONodeWire = nullptr;
    _tempWireCheckPoints.clear();
}

void SimControlManager::TypeLetter(SDL_Keycode key) {
    switch(key){
        case SDLK_BACKSPACE:
            if(_highlightedIONode){
                if(!_highlightedIONode->Tag().empty()){
                    _highlightedIONode->SetTag(_highlightedIONode->Tag().substr(0, _highlightedIONode->Tag().size()-1));
                }
            }
            else if(!_nameBuffer.empty()){
                _nameBuffer = _nameBuffer.substr(0, _nameBuffer.size()-1);
            }
            break;
        case SDLK_SPACE:
            if(_highlightedIONode)
                _highlightedIONode->SetTag(_highlightedIONode->Tag() + " ");
            else
                _nameBuffer.append(" ");
            break;
        default:
            // Add name to buffer if name of key is only character long
            std::string letter = SDL_GetKeyName(key);
            if(letter.size() == 1){
                if(_highlightedIONode){
                    _highlightedIONode->SetTag(_highlightedIONode->Tag() + letter);
                }
                else{
                    _nameBuffer.append(letter);
                }
            }
            break;
    }
}

const std::string& SimControlManager::NameBuffer() const {
    return _nameBuffer;
}

void SimControlManager::ResetNameBuffer() {
    _nameBuffer.clear();
}

void SimControlManager::SetHighlightedIONode(IONode* node) {
    _highlightedIONode = node;
}

void SimControlManager::PlaceWireCheckPoint(const Vector2& checkPoint) {
    if(!_placingWire) return;
    _tempWireCheckPoints.push_back(checkPoint);
}

const std::vector<Vector2>& SimControlManager::TempWireCheckPoints() const {
    return _tempWireCheckPoints;
}
