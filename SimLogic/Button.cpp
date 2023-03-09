//
// Created by Conner on 3/5/2023.
//

#include "Button.h"
#include <utility>

#define CharacterSpace 15
#define ButtonHeight 25

Button::Button(std::string text, Vector2 position, std::function<void()> function)
: _text{std::move(text)}, _function{std::move(function)} {
    if(_text.size() < 4){
        _position = position;
        _extends = {50, ButtonHeight};
    }
    else{
        int boxWidth = CharacterSpace * _text.size();
        _position = {position.x, position.y};
        _extends = {boxWidth, ButtonHeight};
    }
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

std::string Button::Text() const {
    return _text;
}
