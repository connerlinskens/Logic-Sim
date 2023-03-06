//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_MOUSECOLLISIONMANAGER_H
#define LOGIC_SIM_MOUSECOLLISIONMANAGER_H


#include <list>
#include <memory>
#include "../Data/IClickable.h"
#include "../SimLogic/Wire.h"

class MouseCollisionManager {
public:
    void AddClickable(IClickable* clickable){
        _activeClickables.push_back(clickable);
        _activeClickables.sort(CompareClickablePriority);
    }
    void RemoveClickable(IClickable* clickable){
        _activeClickables.remove(clickable);
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
    std::list<IClickable*> _activeClickables;
};


#endif //LOGIC_SIM_MOUSECOLLISIONMANAGER_H
