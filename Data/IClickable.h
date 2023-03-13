//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_ICLICKABLE_H
#define LOGIC_SIM_ICLICKABLE_H

#include "Vector2.h"

class IClickable {
public:
    virtual void Clicked() {};
    virtual void HoverEnter()  {};
    virtual void HoverExit() {};
    virtual Vector2 AABBPosition() = 0;
    [[nodiscard]] virtual Vector2 AABBExtends() const = 0;
    [[nodiscard]] virtual int ClickLayer() const { return 0; };
};

#endif //LOGIC_SIM_ICLICKABLE_H
