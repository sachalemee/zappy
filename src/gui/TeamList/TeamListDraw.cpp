/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamListDraw
*/

#include "TeamList.hpp"

void TeamList::drawTitle(float panelX, float panelY, float panelWidth, const char *title) const
{
    int ts = 30;

    DrawText(title, static_cast<int>(panelX + (panelWidth - MeasureText(title, ts)) / 2),
        static_cast<int>(panelY) + 18, ts, RAYWHITE);
}

void TeamList::drawTeamRow(float x, float y, float width, const std::string &name, int count) const
{
    int cw;
    Rectangle box = {x, y, width, 40};
    DrawRectangleRounded(box, 0.2f, 6, Color{45, 48, 58, 255});
    DrawRectangleLinesEx(box, 2, Color{80, 84, 96, 255});

    DrawText(name.c_str(), static_cast<int>(x) + 16,
            static_cast<int>(y) + 10, 20, RAYWHITE);
    const char *countText = TextFormat("%d", count);
    cw = MeasureText(countText, 20);
    DrawText(countText, static_cast<int>(x + box.width) - cw - 16,
            static_cast<int>(y) + 10, 20, LIGHTGRAY);
}

void TeamList::drawPlayerRow(float x, float y, float width, const Player &player) const
{
    Rectangle box = {x, y, width, 40};
    const char *text = TextFormat(
            "#%-3d Lvl %-2d  food:%-2d linemate:%-2d deraumere:%-2d "
            "sibur:%-2d mendiane:%-2d phiras:%-2d thystame:%-2d",
            player.id, player.level, player.inventory[0],
            player.inventory[1], player.inventory[2], player.inventory[3],
            player.inventory[4], player.inventory[5], player.inventory[6]);

    DrawRectangleRounded(box, 0.2f, 6, Color{45, 48, 58, 255});
    DrawRectangleLinesEx(box, 2, Color{80, 84, 96, 255});
    DrawText(text, static_cast<int>(x) + 16, static_cast<int>(y) + 11, 17,
            RAYWHITE);
}

void TeamList::drawBackButton(Rectangle box) const
{
    const char *label = "< Back";

    DrawRectangleRounded(box, 0.3f, 6, Color{60, 64, 76, 255});
    DrawRectangleLinesEx(box, 2, Color{80, 84, 96, 255});
    DrawText(label, static_cast<int>(box.x) + 12, static_cast<int>(box.y) + 8, 18, RAYWHITE);
}

void TeamList::drawTeamPanel(const game_state &state) const
{
    const auto &teams = state.teams();
    const auto &players = state.players();
    Rectangle panel = teamPanelRect(state);

    DrawRectangleRounded(panel, 0.05f, 8, Color{28, 30, 38, 255});
    DrawRectangleLinesEx(panel, 2, Color{60, 64, 76, 255});
    drawTitle(panel.x, panel.y, panel.width, "Teams");
    for (size_t i = 0; i < teams.size(); i++) {
        Rectangle row = teamRowRect(state, i);
        drawTeamRow(row.x, row.y, row.width, teams[i], countPlayersInTeam(players, teams[i]));
    }
}

void TeamList::drawPlayerPanel(const game_state &state) const
{
    const auto &players = state.players();
    Rectangle panel = playerPanelRect(state);
    float y = panel.y + 70;

    DrawRectangleRounded(panel, 0.05f, 8, Color{28, 30, 38, 255});
    DrawRectangleLinesEx(panel, 2, Color{60, 64, 76, 255});
    drawTitle(panel.x, panel.y, panel.width, _selectedTeam.c_str());
    drawBackButton(backButtonRect(state));
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].team != _selectedTeam)
            continue;
        drawPlayerRow(panel.x + 20, y, panel.width - 40, players[i]);
        y += 50;
    }
}
