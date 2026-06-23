/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamListGeometry
*/

#include "TeamList.hpp"

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

Rectangle TeamList::teamPanelRect(const game_state &state) const
{
    float pw = 400;
    float ph = 90 + state.teams().size() * 50.0f;
    float px = (GetScreenWidth() - pw) / 2.0f;
    float py = (GetScreenHeight() - ph) / 2.0f;

    return {px, py, pw, ph};
}

Rectangle TeamList::playerPanelRect(const game_state &state) const
{
    int count = countPlayersInTeam(state.players(), _selectedTeam);
    float pw = 760;
    float ph = 110 + count * 50.0f;
    float px = (GetScreenWidth() - pw) / 2.0f;
    float py = (GetScreenHeight() - ph) / 2.0f;

    return {px, py, pw, ph};
}

Rectangle TeamList::teamRowRect(const game_state &state, size_t index) const
{
    Rectangle panel = teamPanelRect(state);

    return {panel.x + 20, panel.y + 70 + index * 50.0f, panel.width - 40, 40};
}

Rectangle TeamList::backButtonRect(const game_state &state) const
{
    Rectangle panel = playerPanelRect(state);

    return {panel.x + 20, panel.y + 18, 90, 34};
}
