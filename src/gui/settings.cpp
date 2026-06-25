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
    , _draggingMusicSlider(false)
    , _draggingSfxSlider(false)
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

Color Settings::hoverColor(bool over, Color base, Color hover) const
{
    if (over)
        return hover;
    return base;
}

void Settings::drawKeyRow(float x, float y, const char *key, const char *label)
{
    int kw = MeasureText(key, 24);
    float boxWidth = kw + 20 > 44 ? kw + 20 : 44;
    Rectangle box = {x, y, boxWidth, 44};

    DrawRectangleRounded(box, 0.25f, 6, Color{45, 48, 58, 255});
    DrawRectangleLinesEx(box, 2, Color{80, 84, 96, 255});
    DrawText(key, static_cast<int>(x) + (static_cast<int>(boxWidth) - kw) / 2,
         static_cast<int>(y) + 10, 24, RAYWHITE);
    DrawText(label, static_cast<int>(x) + static_cast<int>(boxWidth) + 14,
            static_cast<int>(y) + 12, 22, LIGHTGRAY);
}

void Settings::drawOverlay()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
}

void Settings::drawPanel()
{
    float pw = 760, ph = 700;

    _panel = {(GetScreenWidth() - pw) / 2.0f,
            (GetScreenHeight() - ph) / 2.0f, pw, ph};
    DrawRectangleRounded(_panel, 0.05f, 8, Color{28, 30, 38, 255});
    DrawRectangleLinesEx(_panel, 2, Color{60, 64, 76, 255});
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

    drawKeyRow(x, y, "Z", "Up");
    drawKeyRow(x, y + 60, "S", "Down");
    drawKeyRow(x, y + 120, "D", "Right");
    drawKeyRow(x, y + 180, "Q", "Left");
    drawKeyRow(x, y + 240, "X", "(Un)lock");
    drawKeyRow(x, y + 300, "Tab", "Team list");
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

    drawKeyRow(x, y + 90, "A", "Ascend");
    drawKeyRow(x, y + 150, "E", "Descend");
}

void Settings::drawWindowMode()
{
    float x = _panel.x + 380;
    float y = _panel.y + 110;

    Rectangle btn = {x, y + 300, 220, 46};
    bool over = CheckCollisionPointRec(GetMousePosition(), btn);

    DrawRectangleRounded(btn, 0.3f, 6, over ?
            Color{70, 74, 86, 255} : Color{50, 54, 64, 255});
    DrawRectangleLinesEx(btn, 2, Color{80, 84, 96, 255});
    const char *label = "Window mode";
    DrawText(label, static_cast<int>(btn.x + (btn.width - MeasureText(label, 20)) / 2),
             static_cast<int>(btn.y) + 13, 20, RAYWHITE);

    if (over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !_justOpened) {
        ToggleFullscreen();
        _fullscreen = !_fullscreen;
    }
}

void Settings::drawMiscKeys()
{
    float x = _panel.x + 380;
    float y = _panel.y + 110;

    drawKeyRow(x, y + 220, "L", "Show/hide levels");
}

void Settings::drawFrequencyButtons(Rectangle minus, Rectangle plus,
        int freq, float x, float y)
{
    Color minusColor = hoverColor(
            CheckCollisionPointRec(GetMousePosition(), minus),
            Color{50, 54, 64, 255}, Color{70, 74, 86, 255});
    Color plusColor = hoverColor(
            CheckCollisionPointRec(GetMousePosition(), plus),
            Color{50, 54, 64, 255}, Color{70, 74, 86, 255});

    DrawText("Frequency", static_cast<int>(x), static_cast<int>(y), 22,
            RAYWHITE);
    DrawText(TextFormat("%d", freq), static_cast<int>(x) + 150,
            static_cast<int>(y), 22, RAYWHITE);

    DrawRectangleRounded(minus, 0.25f, 6, minusColor);
    DrawRectangleLinesEx(minus, 2, Color{80, 84, 96, 255});
    DrawText("-", static_cast<int>(minus.x) + 13,
            static_cast<int>(minus.y) + 4, 24, RAYWHITE);

    DrawRectangleRounded(plus, 0.25f, 6, plusColor);
    DrawRectangleLinesEx(plus, 2, Color{80, 84, 96, 255});
    DrawText("+", static_cast<int>(plus.x) + 10,
            static_cast<int>(plus.y) + 4, 24, RAYWHITE);
}

void Settings::handleFrequencyClick(Rectangle minus, Rectangle plus, int freq,
        connection_manager &connection)
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || _justOpened)
        return;
    if (CheckCollisionPointRec(GetMousePosition(), minus) && freq > 1)
        connection.sendCommand("sst " + std::to_string(freq - 1) + "\n");
    else if (CheckCollisionPointRec(GetMousePosition(), plus))
        connection.sendCommand("sst " + std::to_string(freq + 1) + "\n");
}

void Settings::drawFrequency(const game_state &state,
        connection_manager &connection)
{
    float x = _panel.x + 40;
    float y = _panel.y + 110 + 380;
    int freq = state.timeUnit();
    Rectangle minus = {x + 220, y - 6, 36, 36};
    Rectangle plus = {x + 264, y - 6, 36, 36};

    drawFrequencyButtons(minus, plus, freq, x, y);
    handleFrequencyClick(minus, plus, freq, connection);
}

void Settings::handleOutsideClick()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !_justOpened
        && !CheckCollisionPointRec(GetMousePosition(), _panel))
        close();
}

void Settings::drawBody()
{
    drawOverlay();
    drawPanel();
    drawTitle();
    drawCloseButton();
    drawMovementKeys();
    drawSensitivity();
    drawElevationKeys();
    drawWindowMode();
    drawMiscKeys();
    handleOutsideClick();
}

void Settings::draw(AudioManager &audio)
{
    if (!_open)
        return;
    drawBody();
    drawSoundSliders(audio);
    _justOpened = false;
}

void Settings::draw(const game_state &state, connection_manager &connection,
    AudioManager &audio)
{
    if (!_open)
        return;
    drawBody();
    drawFrequency(state, connection);
    drawSoundSliders(audio);
    _justOpened = false;
}

void Settings::drawVolumeSlider(float x, float y, const char *label,
    float &value, bool &dragging)
{
    Vector2 m = GetMousePosition();
    Rectangle track = {x, y + 42, 300, 6};
    float ratio = value;
    Rectangle knob = {track.x + ratio * track.width - 9, track.y - 8, 18, 22};
    Rectangle zone = {track.x - 10, track.y - 14, track.width + 20, 34};

    DrawText(label, static_cast<int>(x), static_cast<int>(y), 22, RAYWHITE);
    DrawText(TextFormat("%d", static_cast<int>(value * 100)),
        static_cast<int>(x) + 230, static_cast<int>(y), 22, RAYWHITE);
    DrawRectangleRounded(track, 1.0f, 4, Color{70, 74, 86, 255});
    DrawRectangleRounded(knob, 0.4f, 6, Color{46, 204, 113, 255});

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
        && CheckCollisionPointRec(m, zone) && !_justOpened)
        dragging = true;
    if (dragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float rel = (m.x - track.x) / track.width;
        if (rel < 0) rel = 0;
        if (rel > 1) rel = 1;
        value = rel;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        dragging = false;
}

void Settings::drawSoundSliders(AudioManager &audio)
{
    float x = _panel.x + 40;
    float y = _panel.y + 110 + 420;
    float musicVol = audio.getMusicVolume();
    float sfxVol = audio.getVolume();

    drawVolumeSlider(x, y, "Music", musicVol, _draggingMusicSlider);
    drawVolumeSlider(x + 360, y, "Effects", sfxVol, _draggingSfxSlider);
    audio.setMusicVolume(musicVol);
    audio.setVolume(sfxVol);
}