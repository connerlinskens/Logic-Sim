//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_LOGICBLOCK_H
#define LOGIC_SIM_LOGICBLOCK_H

#include <utility>
#include <vector>
#include <string>
#include "IONode.h"
#include "../Data/Vector2.h"
#include "../Data/ChipDrawData.h"
#include "../Data/IClickable.h"
#include "../SimManagers/MouseCollisionManager.h"

#define NodeHeightStep 30

enum class ChipType{
    AND,
    OR,
    NOT,
    PROGRAMMABLE
};

class Chip : public IClickable{
public:
    Chip(std::string name, int inputs, int outputs, Color color = {});

    virtual ~Chip() = default;
    virtual void Execute() = 0;

    void SetPosition(const Vector2& position);
    Vector2 Position() { return _position; }

    void RegisterToCollisionManager(MouseCollisionManager& mouseCollisionManager, bool RegisterChip = true, bool RegisterNodes = true);

    ChipDrawData GetChipDrawData(){
        return ChipDrawData{_name, _position, _extends, _color};
    }

    Vector2 AABBPosition() override{
        Vector2 extends = AABBExtends();
        return {Position().x - extends.x/2, Position().y - extends.y/2};

    }
    [[nodiscard]] Vector2 AABBExtends() const override { return _extends; }
    [[nodiscard]] int ClickLayer() const override { return 1; }

    [[nodiscard]] const std::vector<std::unique_ptr<IONode>>& Inputs() const;
    [[nodiscard]] const std::vector<std::unique_ptr<IONode>>& Outputs() const;

    void RemoveIONode(IONode* node, bool input);

    void RepositionIONodes();
    void RepositionIONodesForInternalView(Vector2 windowSize);

    void ResizeChipToFitName();

    [[nodiscard]] const std::string& Name() const;
    void SetName(const std::string& name);
    [[nodiscard]] Color GetColor() const;
    void SetColor(Color color);
protected:
    std::vector<std::unique_ptr<IONode>> _inputs;
    std::vector<std::unique_ptr<IONode>> _outputs;

    std::string _name;
    Vector2 _position;
    Vector2 _extends;

    Color _color;
};


#endif //LOGIC_SIM_LOGICBLOCK_H
