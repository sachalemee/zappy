/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu
*/

#pragma once
#include <raylib.h>
#include "menu_gui/TextBox.hpp"

class Menu {
public:
    Menu();
    ~Menu();
    void draw();
private:
    Texture2D _background;
    TextBox _hostInput;
    TextBox _portInput;
};
