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
    }
    void RemoveClickables(IClickable* clickable){
        _currentClickables.remove(clickable);
    }
    void ClearClickables(){
        _currentClickables.clear();
    }

    IClickable* CheckMouseCollision(int mouseX, int mouseY);
private:
    std::list<IClickable*> _currentClickables;
};


#endif //LOGIC_SIM_MOUSECOLLISIONMANAGER_H
