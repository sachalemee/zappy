/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TextBox
*/

#include "menu_gui/TextBox.hpp"

TextBox::TextBox(float x, float y, float w, float h,
                 const std::string &placeholder)
    : _rect{x, y, w, h}
    , _value("")
    , _placeholder(placeholder)
    , _focused(false)
    , _blink(0.f)
{}

void TextBox::setRect(Rectangle r)
{
    _rect = r;
}

void TextBox::update()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        _focused = CheckCollisionPointRec(GetMousePosition(), _rect);

    if (!_focused)
        return;

    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125 && _value.size() < 64)
            _value += static_cast<char>(key);
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !_value.empty())
        _value.pop_back();

    _blink += GetFrameTime();
    if (_blink >= 1.0f)
        _blink = 0.f;
}

void TextBox::draw()
{
    Color bg = {45, 48, 58, 255};
    Color border = _focused ? Color{46, 204, 113, 255} : Color{70, 74, 86, 255};

    DrawRectangleRounded(_rect, 0.3f, 6, bg);
    DrawRectangleLinesEx(_rect, 2, border);
    
    int fontSize = 20;
    int tx = static_cast<int>(_rect.x) + 14;
    int ty = static_cast<int>(_rect.y + (_rect.height - fontSize) / 2);

    if (_value.empty() && !_focused) {
        DrawText(_placeholder.c_str(), tx, ty, fontSize, Color{120, 124, 134, 255});
        return;
    }
    DrawText(_value.c_str(), tx, ty, fontSize, RAYWHITE);
    if (_focused && _blink < 0.5f) {
        int w = MeasureText(_value.c_str(), fontSize);
        DrawText("|", tx + w + 2, ty, fontSize, RAYWHITE);
    }
}

const std::string &TextBox::getValue() const
{
    return _value;
}

void TextBox::setValue(const std::string &v)
{
    _value = v;
}