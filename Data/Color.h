//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_COLOR_H
#define LOGIC_SIM_COLOR_H

#include <cstdint>

struct Color{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
    Color() : Color(255,255,255,255){};
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : red(red), green(green), blue(blue), alpha(alpha) {}
};

#endif //LOGIC_SIM_COLOR_H
