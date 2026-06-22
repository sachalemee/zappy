/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SettingsButton
*/

#ifndef SETTINGSBUTTON_HPP_
#define SETTINGSBUTTON_HPP_

#include <raylib.h>
#include "settings.hpp"

class SettingsButton {
public:
    void draw(Settings &settings);

private:
    Rectangle buttonRect() const;
    void drawButton(Rectangle btn) const;
    void handleClicks(Rectangle btn, Settings &settings) const;
};

#endif /* !SETTINGSBUTTON_HPP_ */
