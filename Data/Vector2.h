//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_VECTOR2_H
#define LOGIC_SIM_VECTOR2_H

#include <cmath>

struct Vector2{
    int x;
    int y;
    Vector2() : Vector2(0,0){}
    Vector2(int x, int y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const { return {this->x + other.x, this->y + other.y}; }
    Vector2 operator+=(const Vector2& other) {this->x += other.x; this->y += other.y; return *this; }
    Vector2 operator-(const Vector2& other) const { return {this->x - other.x, this->y - other.y}; }
    Vector2 operator-=(const Vector2& other) {this->x -= other.x; this->y -= other.y; return *this; }

    bool operator==(const Vector2& other) const {return this->x == other.x && this->y == other.y; }

    static float Distance(const Vector2& a, const Vector2& b) { return sqrtf(powf(static_cast<float>(b.x - a.x), 2) + powf(static_cast<float>(b.y - a.y), 2)); }
};


#endif //LOGIC_SIM_VECTOR2_H
