/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu
*/

#ifndef MENU_HPP_
#define MENU_HPP_

#include <raylib.h>
#include <string>
#include "menu_gui/TextBox.hpp"
#include "settings.hpp"

class Menu {
public:
    Menu();
    ~Menu();

    void draw();
    bool isConnectPressed() const;
    const std::string &getHost() const;
    const std::string &getPort() const;

private:
    void layout();
    void drawBackground() const;
    void drawCard() const;
    void drawTitle() const;
    void drawLabels() const;
    void drawInputs();
    void drawConnectButton() const;
    void drawSettingsButton() const;
    void handleClicks();
    bool isMouseOver(Rectangle r) const;

    Texture2D _background;
    bool _hasBackground;
    TextBox _hostInput;
    TextBox _portInput;
    Rectangle _card;
    Rectangle _connectBtn;
    Rectangle _settingsBtn;
    bool _connectPressed;
    Settings _settings;
};

#endif /* MENU_HPP_ */