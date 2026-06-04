/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TextBox
*/
#include <raylib.h>
#include "menu_gui/TextBox.hpp"
#include <iostream>
TextBox::TextBox(float x, float y, float width, float height)
    : _rect{x, y, width, height}
{}

void TextBox::draw()
{
    DrawRectangleRec(_rect, LIGHTGRAY);
}

const std::string &TextBox::getValue() const
{
    return _value;
}

bool TextBox::mouseIsOver() const
{
    return CheckCollisionPointRec(GetMousePosition(), _rect);
}

void TextBox::update()
{
    int key = 0;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        _focused = mouseIsOver();
    if (!_focused)
        return;
    key = GetCharPressed();
    while (key > 0) {
        if (key >= 32) {
            _value += char(key);
            std::cout << "Key pressed: " << char(key) << std::endl;
            std::cout << "Current value: " << _value << std::endl;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !_value.empty())
        _value.pop_back();
}