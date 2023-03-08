//
// Created by Conner on 3/4/2023.
//

#include "ChipAND.h"

ChipAND::ChipAND(Color color) : Chip("AND", 2, 1, color) {
}

void ChipAND::Execute() {
    bool result = (_inputs[0]->State() && _inputs[1]->State());
    _outputs[0]->SetState(result);
}

