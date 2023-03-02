//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_IONODE_H
#define LOGIC_SIM_IONODE_H

#include "../Data/IClickable.h"

#define NodeSize 20

class IONode : public IClickable{
public:
    explicit IONode(Vector2 startPos = Vector2(), bool manuallyOverridable = true);
    void ChangeState();
    [[nodiscard]] bool State() const;

    [[nodiscard]] bool IsManuallyOverridable() const;
    void SetManuallyOverridableState(bool state);

    void Translate(Vector2 translation);
    Vector2 Position();

    Vector2 AABBPosition() override;
    Vector2 AABBExtends() override;
    int clickLayer() override;
    void Clicked() override;

protected:
    bool _state;
    bool _manuallyOverridable;
    Vector2 _position;
};

#endif //LOGIC_SIM_IONODE_H
