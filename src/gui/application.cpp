/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** application
*/

#include "application.hpp"

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
    if (port > 0 && _connection.connect(_menu.getHost(), port))
        _inGame = true;
}

void application::updateMenu()
{
    _menu.draw();
    if (_menu.isConnectPressed())
        connectFromMenu();
}

void application::updateGame()
{
    _connection.update(_state);
    if (!_settings.isOpen())
        _camera.update(50.0f);
    if (!_settings.isOpen())
        _teamList.update(_state);
    _renderer.draw(_state, _camera.get());
    _zoomButtons.draw(_camera);
    _settingsButton.draw(_settings);
    _settings.draw();
    _teamList.draw(_state);
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
