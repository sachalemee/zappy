/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** application
*/

#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "Window.hpp"
#include "menu_gui/Menu.hpp"
#include "network/network_client.hpp"
#include <string>

class application {
public:
    application();
    void run();

private:
    void updateMenu();
    void updateGame();
    void connectFromMenu();
    void handleLine(const std::string &line);

    Window _window;
    Menu _menu;
    network_client _net;
    bool _inGame = false;
    bool _handshakeDone = false;
};

#endif /* APPLICATION_HPP_ */