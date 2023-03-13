//
// Created by Conner on 3/5/2023.
//

#ifndef LOGIC_SIM_BUTTON_H
#define LOGIC_SIM_BUTTON_H

#include <string>
#include <functional>
#include "../Data/IClickable.h"
#include "../Data/Color.h"

#define DefaultBackgroundColor Color (70, 70, 70, 255)
#define DefaultBackgroundHighlightedColor Color (100, 100, 100, 255)

class Button : public IClickable {
public:
    explicit Button(std::string text, Vector2 position, std::function<void(const std::string& name)> function);
    ~Button() = default;

    [[nodiscard]] Vector2 Position() const;
    [[nodiscard]] std::string Text() const;

    void SetPosition(Vector2 position);

    Vector2 AABBPosition() override;
    [[nodiscard]] Vector2 AABBExtends() const override;
    [[nodiscard]] int ClickLayer() const override;
    void Clicked() override;

    void HoverEnter() override;

    void HoverExit() override;

    [[nodiscard]] const Color& BackgroundColor() const;

private:
    std::string _text;
    Vector2 _position;
    Vector2 _extends;
    std::function<void(const std::string& name)> _function;
    Color _backgroundColor;
};


#endif //LOGIC_SIM_BUTTON_H
