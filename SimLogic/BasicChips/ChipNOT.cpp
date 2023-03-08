//
// Created by Conner on 3/4/2023.
//

#include "ChipNOT.h"

ChipNOT::ChipNOT(Color color)  : Chip("NOT", 1, 1, color) {
}

void ChipNOT::Execute() {
    bool result = !_inputs[0]->State();
    _outputs[0]->SetState(result);
}
