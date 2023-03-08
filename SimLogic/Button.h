//
// Created by Conner on 3/5/2023.
//

#ifndef LOGIC_SIM_BUTTON_H
#define LOGIC_SIM_BUTTON_H

#include <string>
#include <functional>
#include "../Data/IClickable.h"

class Button : public IClickable {
public:
    explicit Button(std::string text, Vector2 position, Vector2 extends, std::function<void()> function);
    ~Button() = default;

    [[nodiscard]] Vector2 Position() const;
    [[nodiscard]] std::string Text() const;

    Vector2 AABBPosition() override;
    [[nodiscard]] Vector2 AABBExtends() const override;
    [[nodiscard]] int ClickLayer() const override;
    void Clicked() override;
private:
    std::string _text;
    Vector2 _position;
    Vector2 _extends;
    std::function<void()> _function;
};


#endif //LOGIC_SIM_BUTTON_H
