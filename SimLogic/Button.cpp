//
// Created by Conner on 3/5/2023.
//

#include "Button.h"
#include <utility>

#define CharacterSpace 15
#define ButtonHeight 25

Button::Button(std::string text, Vector2 position, std::function<void(const std::string& name)> function)
: _text{std::move(text)}, _function{std::move(function)}, _backgroundColor{DefaultBackgroundColor} {
    if(_text.size() < 4){
        _extends = {3 * CharacterSpace, ButtonHeight};
    }
    else{
        int boxWidth = CharacterSpace * (int)_text.size();
        _extends = {boxWidth, ButtonHeight};
    }
    _position = position;
}

void Button::Clicked() {
    _function(_text);
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

void Button::SetPosition(Vector2 position) {
    _position = position;
}

const Color &Button::BackgroundColor() const {
    return _backgroundColor;
}

void Button::HoverEnter() {
    _backgroundColor = DefaultBackgroundHighlightedColor;
}

void Button::HoverExit() {
    _backgroundColor = DefaultBackgroundColor;
}
