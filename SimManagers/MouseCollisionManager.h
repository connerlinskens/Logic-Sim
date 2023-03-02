//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_MOUSECOLLISIONMANAGER_H
#define LOGIC_SIM_MOUSECOLLISIONMANAGER_H


#include <list>
#include "../Data/IClickable.h"

class MouseCollisionManager {
public:
    void AddClickable(IClickable* clickable){
        _currentClickables.push_back(clickable);
        _currentClickables.sort(CompareClickablePriority);
    }
    void RemoveClickable(IClickable* clickable){
        _currentClickables.remove(clickable);
    }
    void ClearClickables(){
        _currentClickables.clear();
    }

    IClickable* CheckMouseCollision(int mouseX, int mouseY);
private:
    static bool CompareClickablePriority(const IClickable* left, const IClickable* right){
        return left->ClickLayer() > right->ClickLayer();
    }
private:
    std::list<IClickable*> _currentClickables;
};


#endif //LOGIC_SIM_MOUSECOLLISIONMANAGER_H
