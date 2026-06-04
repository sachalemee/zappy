/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TextBox
*/

#include "menu_gui/TextBox.hpp"

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
