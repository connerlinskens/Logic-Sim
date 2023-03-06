//
// Created by Conner on 3/5/2023.
//

#include "Button.h"
#include <utility>

Button::Button(std::string text, Vector2 position, Vector2 extends, std::function<void()> function)
: _text{std::move(text)}, _position{position}, _extends{extends}, _function{std::move(function)} {
}

void Button::Clicked() {
    _function();
}

Vector2 Button::AABBPosition() {
    return {_position.x - (_extends.x/2), _position.y - (_extends.y/2)};
}

Vector2 Button::AABBExtends() const {
    return _extends;
}

int Button::ClickLayer() const {
    return 5;
}

Vector2 Button::Position() const {
    return _position;
}
