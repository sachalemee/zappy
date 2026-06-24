/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Settings
*/

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <raylib.h>
#include "game/gamestate.hpp"
#include "network/connection_manager.hpp"

class Settings {
public:
    Settings();

    void draw();
    void draw(const game_state &state, connection_manager &connection);
    void open();
    void close();
    bool isOpen() const;
    float getSensitivity() const;

private:
    void drawOverlay();
    void drawPanel();
    void drawTitle();
    void drawCloseButton();
    void drawMovementKeys();
    void drawSensitivity();
    void drawElevationKeys();
    void drawWindowMode();
    void drawMiscKeys();
    void drawFrequency(const game_state &state, connection_manager &connection);
    void drawFrequencyButtons(Rectangle minus, Rectangle plus,
            int freq, float x, float y);
    void handleFrequencyClick(Rectangle minus, Rectangle plus, int freq,
            connection_manager &connection);
    void handleOutsideClick();
    void drawKeyRow(float x, float y, const char *key, const char *label);
    void drawBody();
    Color hoverColor(bool over, Color base, Color hover) const;

    bool _open;
    bool _justOpened;
    bool _draggingSlider;
    bool _fullscreen;
    float _sensitivity;
    Rectangle _panel;
};

#endif /* SETTINGS_HPP_ */