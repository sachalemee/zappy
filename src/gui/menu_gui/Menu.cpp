/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu
*/

#include "menu_gui/Menu.hpp"

Menu::Menu()
    : _background(LoadTexture("src/gui/menu_gui/img/background.jpg"))
    , _hasBackground(_background.id != 0)
    , _hostInput(0, 0, 0, 0, "xxx.x.x.x")
    , _portInput(0, 0, 0, 0, "xxxx")
    , _card{0, 0, 0, 0}
    , _connectBtn{0, 0, 0, 0}
    , _settingsBtn{0, 0, 0, 0}
    , _connectPressed(false)
{
}

Menu::~Menu()
{
    if (_hasBackground)
        UnloadTexture(_background);
}

bool Menu::isMouseOver(Rectangle r) const
{
    return CheckCollisionPointRec(GetMousePosition(), r);
}

bool Menu::isConnectPressed() const
{
    return _connectPressed;
}

const std::string &Menu::getHost() const
{
    return _hostInput.getValue();
}

const std::string &Menu::getPort() const
{
    return _portInput.getValue();
}

void Menu::layout()
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float cw = 460;
    float ch = 470;
    float cx = (sw - cw) / 2.0f;
    float cy = (sh - ch) / 2.0f;
    float ix = cx + 40;
    float iw = cw - 80;

    _card = {cx, cy, cw, ch};
    _hostInput.setRect({ix, cy + 150, iw, 48});
    _portInput.setRect({ix, cy + 240, iw, 48});
    _connectBtn = {cx + (cw - 200) / 2, cy + 330, 200, 50};
    _settingsBtn = {cx + 40, cy + 396, cw - 80, 44};
}

void Menu::drawBackground() const
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    if (_hasBackground) {
        Rectangle src = {0, 0, (float)_background.width,
            (float)_background.height};
        Rectangle dst = {0, 0, (float)sw, (float)sh};
        DrawTexturePro(_background, src, dst, {0, 0}, 0, WHITE);
        DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.45f));
    } else {
        DrawRectangleGradientV(0, 0, sw, sh,
            Color{18, 20, 28, 255}, Color{32, 36, 48, 255});
    }
}

void Menu::drawCard() const
{
    DrawRectangleRounded(_card, 0.06f, 8, Color{28, 30, 38, 235});
    DrawRectangleRoundedLinesEx(_card, 0.06f, 8, 2,
        Color{60, 64, 76, 255});
}

void Menu::drawTitle() const
{
    const char *title = "ZAPPY";
    int size = 40;
    int tw = MeasureText(title, size);
    int x = static_cast<int>(_card.x + (_card.width - tw) / 2);
    int y = static_cast<int>(_card.y) + 36;
    int lineX = static_cast<int>(_card.x + _card.width / 2) - 40;
    int lineY = static_cast<int>(_card.y) + 90;

    DrawText(title, x, y, size, RAYWHITE);
    DrawRectangle(lineX, lineY, 80, 3, Color{46, 204, 113, 255});
}

void Menu::drawLabels() const
{
    int x = static_cast<int>(_card.x) + 40;
    int hostY = static_cast<int>(_card.y) + 128;
    int portY = static_cast<int>(_card.y) + 218;

    DrawText("Host", x, hostY, 18, LIGHTGRAY);
    DrawText("Port", x, portY, 18, LIGHTGRAY);
}

void Menu::drawInputs()
{
    if (!_settings.isOpen()) {
        _hostInput.update();
        _portInput.update();
    }
    _hostInput.draw();
    _portInput.draw();
}

void Menu::drawConnectButton() const
{
    const char *text = "Connect";
    Color base = {46, 204, 113, 255};
    Color hover = {39, 174, 96, 255};
    Color color = isMouseOver(_connectBtn) ? hover : base;
    int tw = MeasureText(text, 22);
    int x = static_cast<int>(_connectBtn.x + (_connectBtn.width - tw) / 2);
    int y = static_cast<int>(_connectBtn.y) + 14;

    DrawRectangleRounded(_connectBtn, 0.3f, 8, color);
    DrawText(text, x, y, 22, WHITE);
}

void Menu::drawSettingsButton() const
{
    const char *text = "Settings";
    Color base = {38, 41, 50, 255};
    Color hover = {50, 54, 64, 255};
    Color color = isMouseOver(_settingsBtn) ? hover : base;
    int tw = MeasureText(text, 20);
    int x = static_cast<int>(_settingsBtn.x +
        (_settingsBtn.width - tw) / 2);
    int y = static_cast<int>(_settingsBtn.y) + 12;

    DrawRectangleRounded(_settingsBtn, 0.3f, 8, color);
    DrawRectangleRoundedLinesEx(_settingsBtn, 0.3f, 8, 2,
        Color{70, 74, 86, 255});
    DrawText(text, x, y, 20, LIGHTGRAY);
}

void Menu::handleClicks()
{
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (!_settings.isOpen() && clicked) {
        if (isMouseOver(_connectBtn))
            _connectPressed = true;
        else if (isMouseOver(_settingsBtn))
            _settings.open();
    }
}

void Menu::draw()
{
    _connectPressed = false;
    layout();
    drawBackground();
    drawCard();
    drawTitle();
    drawLabels();
    drawInputs();
    drawConnectButton();
    drawSettingsButton();
    handleClicks();
    _settings.draw();
}