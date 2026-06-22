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
#include "ZoomButtons.hpp"
#include "SettingsButton.hpp"
#include "settings.hpp"
#include "TeamList.hpp"
#include "audio/audio_manager.hpp"
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
    AudioManager _audio;
    Menu _menu;
    connection_manager _connection;
    game_state _state;
    renderer _renderer;
    game_camera _camera;
    ZoomButtons _zoomButtons;
    SettingsButton _settingsButton;
    Settings _settings;
    TeamList _teamList;
    bool _inGame = false;
    
};

#endif /* APPLICATION_HPP_ */