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
        const std::string &getValue() const;
    private:
        Rectangle _rect;
        std::string _value;
};
