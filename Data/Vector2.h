//
// Created by Conner on 2/28/2023.
//

#ifndef LOGIC_SIM_VECTOR2_H
#define LOGIC_SIM_VECTOR2_H

struct Vector2{
    int x;
    int y;
    Vector2() : Vector2(0,0){}
    Vector2(int x, int y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const { return {this->x + other.x, this->y + other.y}; }
    Vector2 operator+=(const Vector2& other) {this->x += other.x; this->y += other.y; return *this; }
    Vector2 operator-(const Vector2& other) const { return {this->x - other.x, this->y - other.y}; }
    Vector2 operator-=(const Vector2& other) {this->x -= other.x; this->y -= other.y; return *this; }
};


#endif //LOGIC_SIM_VECTOR2_H
