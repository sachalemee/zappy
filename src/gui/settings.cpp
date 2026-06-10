/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Settings
*/

#include "settings.hpp"

Settings::Settings()
    : _open(false)
    , _justOpened(false)
    , _draggingSlider(false)
    , _fullscreen(false)
    , _sensitivity(319.f)
    , _panel{0, 0, 0, 0}
{}

void Settings::open()
{
    _open = true;
    _justOpened = true;
}

void Settings::close()
{
    _open = false;
}

bool Settings::isOpen() const
{
    return _open;
}

float Settings::getSensitivity() const
{
    return _sensitivity;
}

void Settings::drawKeyRow(float x, float y, const char *key, const char *label)
{
    Rectangle box = {x, y, 44, 44};
    DrawRectangleRounded(box, 0.25f, 6, Color{45, 48, 58, 255});
    DrawRectangleRoundedLinesEx(box, 0.25f, 6, 2, Color{80, 84, 96, 255});

    int kw = MeasureText(key, 24);
    DrawText(key, static_cast<int>(x) + (44 - kw) / 2,
         static_cast<int>(y) + 10, 24, RAYWHITE);
    DrawText(label, static_cast<int>
            (x) + 58, static_cast<int>(y) + 12, 22, LIGHTGRAY);
}

void Settings::drawOverlay()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
}

void Settings::drawPanel()
{
    float pw = 760, ph = 470;

    _panel = {(GetScreenWidth() - pw) / 2.0f,
            (GetScreenHeight() - ph) / 2.0f, pw, ph};
    DrawRectangleRounded(_panel, 0.05f, 8, Color{28, 30, 38, 255});
    DrawRectangleRoundedLinesEx(_panel, 0.05f, 8, 2, Color{60, 64, 76, 255});
}

void Settings::drawTitle()
{
    const char *title = "Settings";
    int ts = 38;

    DrawText(title,
            static_cast<int>
                (_panel.x + (_panel.width - MeasureText(title, ts)) / 2),
            static_cast<int>(_panel.y) + 24, ts, RAYWHITE);
}

void Settings::drawCloseButton()
{
    Rectangle btn = {_panel.x + _panel.width - 56, _panel.y + 20, 36, 36};
    bool over = CheckCollisionPointRec(GetMousePosition(), btn);

    DrawRectangleRounded(btn, 0.3f, 6, over ? Color{200, 60, 60, 255} :
            Color{60, 64, 76, 255});
    DrawText("X", static_cast<int>(btn.x) + 12,
        static_cast<int>(btn.y) + 8, 22, RAYWHITE);

    if (over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !_justOpened)
        close();
}

void Settings::drawMovementKeys()
{
    float x = _panel.x + 40;
    float y = _panel.y + 110;

    drawKeyRow(x, y,        "Z", "Up");
    drawKeyRow(x, y + 60,   "S", "Down");
    drawKeyRow(x, y + 120,  "D", "Right");
    drawKeyRow(x, y + 180,  "Q", "Left");
    drawKeyRow(x, y + 240,  "X", "(Un)lock");
}

void Settings::drawSensitivity()
{
    float x = _panel.x + 380;
    float y = _panel.y + 110;

    Vector2 m = GetMousePosition();

    DrawText("Sensitivity",
            static_cast<int>(x), static_cast<int>(y), 22, RAYWHITE);
    DrawText(TextFormat("%i", static_cast<int>(_sensitivity)),
             static_cast<int>(x) + 230, static_cast<int>(y), 22, RAYWHITE);

    Rectangle track = {x, y + 42, 300, 6};
    DrawRectangleRounded(track, 1.0f, 4, Color{70, 74, 86, 255});

    float ratio = _sensitivity / 500.0f;
    Rectangle knob = {track.x + ratio * track.width - 9, track.y - 8, 18, 22};
    DrawRectangleRounded(knob, 0.4f, 6, Color{46, 204, 113, 255});

    Rectangle zone = {track.x - 10, track.y - 14, track.width + 20, 34};
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(m, zone) && !_justOpened)
        _draggingSlider = true;

    if (_draggingSlider && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float rel = (m.x - track.x) / track.width;
        if (rel < 0)
            rel = 0;
        if (rel > 1)
            rel = 1;
        _sensitivity = rel * 500.0f;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        _draggingSlider = false;
}

void Settings::drawElevationKeys()
{
    float x = _panel.x + 380;
    float y = _panel.y + 110;

    drawKeyRow(x, y + 90,  "A", "Ascend");
    drawKeyRow(x, y + 150, "E", "Descend");
}

void Settings::drawWindowMode()
{
    float x = _panel.x + 380;
    float y = _panel.y + 110;
    
    Rectangle btn = {x, y + 220, 220, 46};
    bool over = CheckCollisionPointRec(GetMousePosition(), btn);

    DrawRectangleRounded(btn, 0.3f, 6, over ?
            Color{70, 74, 86, 255} : Color{50, 54, 64, 255});
    DrawRectangleRoundedLinesEx(btn, 0.3f, 6, 2, Color{80, 84, 96, 255});
    const char *label = "Window mode";
    DrawText(label, static_cast<int>(btn.x + (btn.width - MeasureText(label, 20)) / 2),
             static_cast<int>(btn.y) + 13, 20, RAYWHITE);

    if (over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !_justOpened) {
        ToggleFullscreen();
        _fullscreen = !_fullscreen;
    }
}

void Settings::handleOutsideClick()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !_justOpened
        && !CheckCollisionPointRec(GetMousePosition(), _panel))
        close();
}

void Settings::draw()
{
    if (!_open)
        return;

    drawOverlay();
    drawPanel();
    drawTitle();
    drawCloseButton();
    drawMovementKeys();
    drawSensitivity();
    drawElevationKeys();
    drawWindowMode();
    handleOutsideClick();

    _justOpened = false;
}