/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Settings
*/

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <raylib.h>

class Settings {
public:
    Settings();

    void draw();
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
    void handleOutsideClick();
    void drawKeyRow(float x, float y, const char *key, const char *label);

    bool _open;
    bool _justOpened;
    bool _draggingSlider;
    bool _fullscreen;
    float _sensitivity;
    Rectangle _panel;
};

#endif /* SETTINGS_HPP_ */