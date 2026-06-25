/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** application
*/

#include "application.hpp"

application::application()
    : _window(1920, 1080, "ZAPPY")
{
    _audio.loadMusic("src/gui/audio/sounds/ambient.ogg");
    _audio.setMusicVolume(0.4f);
    _audio.playMusic();
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
    _menu.draw(_audio);
    if (_menu.isConnectPressed())
        connectFromMenu();
}

void application::updateGame()
{
    _connection.update(_state, _audio, _particles);
    _particles.update(GetFrameTime());
    if (!_settings.isOpen())
        _camera.update(50.0f);
    if (!_settings.isOpen())
        _teamList.update(_state, _connection);
    _renderer.draw(_state, _camera.get(), _particles);
    _zoomButtons.draw(_camera);
    _settingsButton.draw(_settings);
    _settings.draw(_state, _connection, _audio);
    _teamList.draw(_state);
}
 
void application::run()
{
    while (!_window.shouldClose()) {
        _window.beginFrame();
        _audio.updateMusic();
        if (_inGame)
            updateGame();
        else
            updateMenu();
        _window.endFrame();
    }
}
