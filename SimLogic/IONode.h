//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_IONODE_H
#define LOGIC_SIM_IONODE_H

#include <vector>
#include <list>
#include "../Data/IClickable.h"
#include "Wire.h"
#include "../Data/Color.h"

#define NodeSize 20
#define NodeOnStateColor Color(210,60,60,255)
#define NodeOnStateHighlightedColor Color(240,90,90,255)
#define NodeOffStateColor Color(40,40,40,255)
#define NodeOffStateHighlightedColor Color(90,90,90,255)

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
    void HoverEnter() override;
    void HoverExit() override;

    void SetState(bool newState);

    void AddWire(Wire* wire);
    void RemoveWire(Wire& wire);
    void ClearWires();
    [[nodiscard]] std::list<Wire*>& Wires();

    [[nodiscard]] int ID() const {return _id;}
    [[nodiscard]] const Color& getColor() const;

protected:
    bool _state;
    bool _manuallyOverridable;
    Vector2 _position;
    enum IONodeType _ioNodeType;
    std::list<Wire*> _wires;
    int _id;
    static int globalIDCounter;
    Color _color;
};

#endif //LOGIC_SIM_IONODE_H
