/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ZoomButtons
*/

#include "ZoomButtons.hpp"

Rectangle ZoomButtons::buttonRect(int index) const
{
    int sw = GetScreenWidth();
    float size = 50;
    float x = sw - size - 20;
    float y = 20 + index * (size + 10);

    return {x, y, size, size};
}

void ZoomButtons::drawButton(Rectangle btn, const char *label) const
{
    Color color = {38, 41, 50, 220};
    if (CheckCollisionPointRec(GetMousePosition(), btn))
        color = {50, 54, 64, 220};
    int tw = MeasureText(label, 28);
    int x = static_cast<int>(btn.x + (btn.width - tw) / 2);
    int y = static_cast<int>(btn.y + (btn.height - 28) / 2);

    DrawRectangleRounded(btn, 0.2f, 8, color);
    DrawRectangleLinesEx(btn, 2, Color{70, 74, 86, 255});
    DrawText(label, x, y, 28, RAYWHITE);
}

void ZoomButtons::handleClicks(Rectangle zoomInBtn, Rectangle zoomOutBtn,
    game_camera &camera) const
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;
    if (CheckCollisionPointRec(GetMousePosition(), zoomInBtn))
        camera.zoomIn();
    else if (CheckCollisionPointRec(GetMousePosition(), zoomOutBtn))
        camera.zoomOut();
}

void ZoomButtons::draw(game_camera &camera)
{
    Rectangle zoomInBtn = buttonRect(0);
    Rectangle zoomOutBtn = buttonRect(1);

    drawButton(zoomInBtn, "+");
    drawButton(zoomOutBtn, "-");
    handleClicks(zoomInBtn, zoomOutBtn, camera);
}
