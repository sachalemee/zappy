/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamList
*/

#include "TeamList.hpp"

TeamList::TeamList()
    : _open(false)
{}

void TeamList::update(const game_state &state)
{
    (void)state;
    if (IsKeyPressed(KEY_TAB))
        _open = !_open;
}

void TeamList::drawTitle(float panelX, float panelY, float panelWidth) const
{
    const char *title = "Teams";
    int ts = 30;

    DrawText(title,
            static_cast<int>(panelX + (panelWidth - MeasureText(title, ts)) / 2),
            static_cast<int>(panelY) + 18, ts, RAYWHITE);
}

void TeamList::drawRow(float x, float y, const std::string &name, int count) const
{
    int cw;
    Rectangle box = {x, y, 360, 40};
    DrawRectangleRounded(box, 0.2f, 6, Color{45, 48, 58, 255});
    DrawRectangleLinesEx(box, 2, Color{80, 84, 96, 255});

    DrawText(name.c_str(), static_cast<int>(x) + 16,
            static_cast<int>(y) + 10, 20, RAYWHITE);
    const char *countText = TextFormat("%d", count);
    cw = MeasureText(countText, 20);
    DrawText(countText, static_cast<int>(x + box.width) - cw - 16,
            static_cast<int>(y) + 10, 20, LIGHTGRAY);
}

int TeamList::countPlayersInTeam(const std::vector<Player> &players,
        const std::string &team) const
{
    int count = 0;

    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].team == team)
            count++;
    }
    return count;
}

void TeamList::drawPanel(const game_state &state) const
{
    const auto &teams = state.teams();
    const auto &players = state.players();
    float pw = 400;
    float ph = 90 + teams.size() * 50.0f;
    float px = (GetScreenWidth() - pw) / 2.0f;
    float py = (GetScreenHeight() - ph) / 2.0f;
    Rectangle panel = {px, py, pw, ph};
    int count = 0;
    float y = 70;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
    DrawRectangleRounded(panel, 0.05f, 8, Color{28, 30, 38, 255});
    DrawRectangleLinesEx(panel, 2, Color{60, 64, 76, 255});
    drawTitle(px, py, pw);

    y += py;
    for (size_t i = 0; i < teams.size(); i++) {
        count = countPlayersInTeam(players, teams[i]);
        drawRow(px + 20, y, teams[i], count);
        y += 50;
    }
}

void TeamList::draw(const game_state &state) const
{
    if (!_open)
        return;
    drawPanel(state);
}
