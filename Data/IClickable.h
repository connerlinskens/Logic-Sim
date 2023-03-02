//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_ICLICKABLE_H
#define LOGIC_SIM_ICLICKABLE_H

#include "Vector2.h"

class IClickable {
public:
    virtual void Clicked() {};
    virtual Vector2 AABBPosition() = 0;
    virtual Vector2 AABBExtends() = 0;
    virtual int clickLayer() { return 0; };
};

#endif //LOGIC_SIM_ICLICKABLE_H
