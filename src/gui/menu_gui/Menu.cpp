/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu
*/

#include "menu_gui/Menu.hpp"

Menu::Menu()
    : _background(LoadTexture("background.jpg")) // add a background of our real project 
    , _hostInput(230, 180, 340, 50)
    , _portInput(230, 260, 340, 50)
{}

Menu::~Menu()
{
    UnloadTexture(_background);
}

void Menu::draw()
{
    _hostInput.update();
    _portInput.update();
    DrawTexture(_background, 0, 0, WHITE);
    DrawText("Welcome on ZAPPY GUI", 100, 100, 50, LIGHTGRAY);
    _hostInput.draw();
    _portInput.draw();
}
