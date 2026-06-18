/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ZoomButtons
*/

#ifndef ZOOMBUTTONS_HPP_
#define ZOOMBUTTONS_HPP_

#include <raylib.h>
#include "gamecamera.hpp"

class ZoomButtons {
public:
    void draw(game_camera &camera);

private:
    Rectangle buttonRect(int index) const;
    void drawButton(Rectangle btn, const char *label) const;
    void handleClicks(Rectangle zoomInBtn, Rectangle zoomOutBtn,
        game_camera &camera) const;
};

#endif /* !ZOOMBUTTONS_HPP_ */
