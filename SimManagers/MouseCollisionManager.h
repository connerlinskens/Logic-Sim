//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_MOUSECOLLISIONMANAGER_H
#define LOGIC_SIM_MOUSECOLLISIONMANAGER_H


#include <list>
#include <memory>
#include <algorithm>
#include "../Data/IClickable.h"
#include "../SimLogic/Wire.h"

class MouseCollisionManager {
public:
    void AddClickable(IClickable* clickable){
        _activeClickables.push_back(clickable);
        std::sort(_activeClickables.begin(), _activeClickables.end(), CompareClickablePriority);
    }
    void RemoveClickable(IClickable* clickable){
        if(clickable == _lastClickable){
            _lastClickable = nullptr;
        }

        auto it = std::find_if(_activeClickables.begin(), _activeClickables.end(),
                               [&](IClickable* c){return c == clickable;});
        if(it != _activeClickables.end()){
            _activeClickables.erase(it);
        }
    }
    void ClearClickables(){
        _activeClickables.clear();
    }

    IClickable* CheckMouseAABBCollision(int mouseX, int mouseY);
    Wire* CheckMouseWireCollision(int mouseX, int mouseY, const std::vector<std::unique_ptr<Wire>>& wires);
private:
    static bool CompareClickablePriority(const IClickable* left, const IClickable* right){
        return left->ClickLayer() > right->ClickLayer();
    }
private:
    std::vector<IClickable*> _activeClickables;
    IClickable* _lastClickable;
};


#endif //LOGIC_SIM_MOUSECOLLISIONMANAGER_H
