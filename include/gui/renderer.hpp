/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** renderer
*/

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "game/gamestate.hpp"
#include <cmath>
#include <raylib.h>
#include <raymath.h>

class renderer {
public:
    renderer();
    ~renderer();
    void draw(const game_state &state, const Camera3D &cam) const;
private:
    void drawTiles(const game_state &state) const;
    void drawTile(int x, int y) const;
    void drawResources(const game_state &state) const;
    void drawTileResources(int x, int y, const game_state &state) const;
    void drawEggs(const game_state &state) const;
    void drawPlayers(const game_state &state) const;
    void drawResource(int i, Vector3 pos) const;
    void drawSky(float light) const;
    void drawGround(const game_state &state, float light, const Camera3D &cam) const;
    void drawCelestials(const game_state &state, float phase) const;
    float _islandScale = 1.0f;
    
    Model _ingotModel;
    Model _breadModel;
    Model _beanModel;
    Model _diamondModel;
    Model _villagerModel;
    Model _eggModel;
    Model _grassModel;
    Model _islandModel;
};

#endif /* !RENDERER_HPP_ */
