/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TextBox
*/

#pragma once
#include <string>
#include <raylib.h>

class TextBox {
    public:
        TextBox(float x, float y, float width, float height);
        void draw();
        void update();
        const std::string &getValue() const;
    private:
        bool mouseIsOver() const;
        Rectangle _rect;
        std::string _value;
        bool _focused = false;
};
