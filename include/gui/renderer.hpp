/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** renderer
*/

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "game/gamestate.hpp"
#include <raylib.h>
 
class renderer {
public:
    void draw(const game_state &state, const Camera3D &cam) const;
 
private:
    void drawTiles(const game_state &state) const;
    void drawTile(int x, int y) const;
    void drawResources(const game_state &state) const;
    void drawTileResources(int x, int y, const game_state &state) const;
    void drawEggs(const game_state &state) const;
    void drawPlayers(const game_state &state) const;
};

#endif /* !RENDERER_HPP_ */
