//
// Created by Conner on 3/4/2023.
//

#include "ChipOR.h"

ChipOR::ChipOR(Color color) : Chip("OR", 2, 1, color) {
}

void ChipOR::Execute() {
    bool result = _inputs[0].State() || _inputs[1].State();
    _outputs[0].SetState(result);
}
