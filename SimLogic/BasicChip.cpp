//
// Created by Conner on 3/3/2023.
//

#include "BasicChip.h"
#include <utility>
#include <iostream>

BasicChip::BasicChip(std::string name, int inputs, int outputs, std::function<bool(bool, bool)> logic, Color color)
: Chip(std::move(name), inputs, outputs, color), _logic{std::move(logic)} {
}

void BasicChip::Execute() {
    bool output;
    if(_inputs.empty()) {
        std::cerr << "Error when executing Basic Chop logic, No Inputs Found, Chip name: " << _name << std::endl;
        return;
    }
    // Most basic chips use 2 inputs
    else if(_inputs.size() > 1){
        output = _logic(_inputs[0].State(), _inputs[1].State());
    }
    // Only one case has 1 inputs, just pass the same bool in both parameters
    else if (_inputs.size() == 1){
        output = _logic(_inputs[0].State(), _inputs[0].State());
    }
    if(_outputs.empty()){
        std::cerr << "Error when executing Basic Chop logic, No Output Found, Chip name: " << _name << std::endl;
        return;
    }
    else{
        _outputs[0].SetState(output);
    }
}


