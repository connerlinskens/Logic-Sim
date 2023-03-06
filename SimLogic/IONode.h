//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_IONODE_H
#define LOGIC_SIM_IONODE_H

#include <vector>
#include <list>
#include "../Data/IClickable.h"
#include "Wire.h"

#define NodeSize 20

enum class IONodeType{
    INPUT,
    OUTPUT
};

class Wire;

class IONode : public IClickable{
public:
    explicit IONode(IONodeType ioNodeType, Vector2 startPos = Vector2(), bool manuallyOverridable = true);
    void ChangeState();
    [[nodiscard]] bool State() const;

    [[nodiscard]] bool IsManuallyOverridable() const;
    void SetManuallyOverridableState(bool state);

    [[nodiscard]] IONodeType IONodeType() const;

    void Translate(Vector2 translation);
    [[nodiscard]] Vector2 Position() const;
    void SetPosition(Vector2 position);

    Vector2 AABBPosition() override;
    [[nodiscard]] Vector2 AABBExtends() const override;
    [[nodiscard]] int ClickLayer() const override;
    void Clicked() override;

    void SetState(bool newState);

    void AddWire(Wire* wire);
    void RemoveWire(Wire& wire);
    void ClearWires();
    [[nodiscard]] const std::list<Wire*>& Wires() const;

    [[nodiscard]] int ID() const {return _id;}
protected:
    bool _state;
    bool _manuallyOverridable;
    Vector2 _position;
    enum IONodeType _ioNodeType;
    std::list<Wire*> _wires;
    int _id;
    static int globalIDCounter;
};

#endif //LOGIC_SIM_IONODE_H
