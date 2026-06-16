/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** renderer
*/

#include "renderer.hpp"

void renderer::drawTile(int x, int y) const
{
    Vector3 pos = {static_cast<float>(x), 0.0f, static_cast<float>(y)};

    DrawCube(pos, 0.95f, 0.1f, 0.95f, Color{34, 139, 34, 255});
    DrawCubeWires(pos, 0.95f, 0.1f, 0.95f, Color{0, 80, 0, 255});
}

void renderer::drawTiles(const game_state &state) const
{
    for (int y = 0; y < state.height(); y++)
        for (int x = 0; x < state.width(); x++)
            drawTile(x, y);
}

void renderer::drawTileResources(int x, int y, const game_state &state) const
{
    static const Color colors[7] = {
        BEIGE, LIGHTGRAY, BROWN, SKYBLUE, PURPLE, ORANGE, GOLD
    };
    const Tile &cell = state.tile(x, y);
    float height = 0.25f;

    for (int i = 0; i < 7; i++) {
        if (cell.resources[i] <= 0)
            continue;
        DrawCube({static_cast<float>(x), height, static_cast<float>(y)},
            0.2f, 0.2f, 0.2f, colors[i]);
        height += 0.25f;
    }
}

void renderer::drawResources(const game_state &state) const
{
    for (int y = 0; y < state.height(); y++)
        for (int x = 0; x < state.width(); x++)
            drawTileResources(x, y, state);
}

void renderer::drawEggs(const game_state &state) const
{
    for (const Egg &egg : state.eggs())
        DrawSphere({static_cast<float>(egg.x), 0.25f,
            static_cast<float>(egg.y)}, 0.2f, WHITE);
}

void renderer::drawPlayers(const game_state &state) const
{
    for (const Player &player : state.players())
        DrawCube({static_cast<float>(player.x), 0.4f,
            static_cast<float>(player.y)}, 0.4f, 0.6f, 0.4f, RED);
}

void renderer::draw(const game_state &state, const Camera3D &cam) const
{
    if (!state.ready()) {
        DrawText("Waiting for server data...", 20, 20, 20, RAYWHITE);
        return;
    }
    BeginMode3D(cam);
    drawTiles(state);
    drawResources(state);
    drawEggs(state);
    drawPlayers(state);
    EndMode3D();
}