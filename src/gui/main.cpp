/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include "zappy.hpp"
#include "Window.hpp"
#include "menu_gui/Menu.hpp"

int main()
{
    Window window(1920, 1080, "ZAPPY GUI");
    Menu menu;

    while (!window.shouldClose()) {
        window.beginFrame();
        menu.draw();
        window.endFrame();
    }
    return IS_EXIT_SUCCESS;
}
