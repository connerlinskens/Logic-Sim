//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_LOGICBLOCK_H
#define LOGIC_SIM_LOGICBLOCK_H

#include <utility>
#include <vector>
#include <string>
#include "Node.h"
#include "../Data/Vector2.h"
#include "../Data/ChipDrawData.h"

class Chip {
public:
    Chip(std::string name, int inputs, int outputs) : _name{std::move(name)}, _position{Vector2()} {
        for(int i = 0; i < inputs; i++){
            _inputs.emplace_back();
        }
        for(int o = 0; o < outputs; o++){
            _outputs.emplace_back();
        }
    };

    virtual ~Chip() = default;
    virtual void Execute() {};

    void SetPosition(const Vector2 position){
        _position = position;
    }
    Vector2 Position() { return _position; }

    ChipDrawData GetChipDrawData(){
        return ChipDrawData{_name, _position, static_cast<int>(_inputs.size()), static_cast<int>(_outputs.size())};
    }
protected:
    std::vector<Node> _inputs;
    std::vector<Node> _outputs;

    std::string _name;
    Vector2 _position;
};


#endif //LOGIC_SIM_LOGICBLOCK_H
