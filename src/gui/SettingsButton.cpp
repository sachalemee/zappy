/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SettingsButton
*/

#include "SettingsButton.hpp"

Rectangle SettingsButton::buttonRect() const
{
    int sw = GetScreenWidth();
    float size = 50;
    float x = sw - size - 20;
    float y = 20 + 2 * (size + 10);

    return {x, y, size, size};
}

void SettingsButton::drawButton(Rectangle btn) const
{
    const char *label = "S";
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

void SettingsButton::handleClicks(Rectangle btn, Settings &settings) const
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;
    if (CheckCollisionPointRec(GetMousePosition(), btn))
        settings.open();
}

void SettingsButton::draw(Settings &settings)
{
    Rectangle btn = buttonRect();

    drawButton(btn);
    if (!settings.isOpen())
        handleClicks(btn, settings);
}
