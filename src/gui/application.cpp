/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** application
*/

#include "application.hpp"
#include <iostream>
 
application::application()
    : _window(1920, 1080, "ZAPPY GUI")
{
}
 
void application::connectFromMenu()
{
    int port = 0;
 
    try {
        port = std::stoi(_menu.getPort());
    } catch (const std::exception &) {
        port = 0;
    }
    if (port > 0 && _net.connectTo(_menu.getHost(), port)) {
        _inGame = true;
        _handshakeDone = false;
    }
}
 
void application::updateMenu()
{
    _menu.draw();
    if (_menu.isConnectPressed())
        connectFromMenu();
}
 
void application::handleLine(const std::string &line)
{
    if (_handshakeDone)
        std::cout << "[SERVER] " << line << std::endl;
    else if (line == "WELCOME") {
        _net.sendLine("GRAPHIC\n");
        _handshakeDone = true;
    }
}
 
void application::updateGame()
{
    for (const std::string &line : _net.poll())
        handleLine(line);
}
 
void application::run()
{
    while (!_window.shouldClose()) {
        _window.beginFrame();
        if (_inGame)
            updateGame();
        else
            updateMenu();
        _window.endFrame();
    }
}
