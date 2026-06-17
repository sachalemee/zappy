/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** application
*/

#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "Window.hpp"
#include "gamecamera.hpp"
#include "menu_gui/Menu.hpp"
#include "network/connection_manager.hpp"
#include "game/gamestate.hpp"
#include "renderer.hpp"
#include <string>

class application {
public:
    application();
    void run();

private:
    void updateMenu();
    void updateGame();
    void connectFromMenu();

    Window _window;
    Menu _menu;
    connection_manager _connection;
    game_state _state;
    renderer _renderer;
    game_camera _camera;
    bool _inGame = false;
};

#endif /* APPLICATION_HPP_ */