/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TextBox
*/

#ifndef TEXTBOX_HPP_
#define TEXTBOX_HPP_

#include <raylib.h>
#include <string>

class TextBox {
public:
    TextBox(float x, float y, float w, float h,
            const std::string &placeholder);

    void setRect(Rectangle r);
    void update();
    void draw();

    const std::string &getValue() const;
    void setValue(const std::string &v);

private:
    Rectangle   _rect;
    std::string _value;
    std::string _placeholder;
    bool        _focused;
    float       _blink;
};

#endif /* TEXTBOX_HPP_ */