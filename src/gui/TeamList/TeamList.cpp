/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamList
*/

#include "TeamList.hpp"

TeamList::TeamList() : _open(false), _selectedTeam(""), _refreshTimer(0)
{}

void TeamList::update(const game_state &state, connection_manager &connection)
{
    if (IsKeyPressed(KEY_TAB)) {
        _open = !_open;
        _selectedTeam.clear();
    }
    if (!_open)
        return;
    if (_selectedTeam.empty()) {
        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            return;
        for (size_t i = 0; i < state.teams().size(); i++) {
            if (CheckCollisionPointRec(GetMousePosition(),
                    teamRowRect(state, i))) {
                _selectedTeam = state.teams()[i];
                _refreshTimer = 0;
                requestInventories(state, connection);
            }
        }
        return;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(GetMousePosition(), backButtonRect(state))) {
        _selectedTeam.clear();
        return;
    }
    _refreshTimer++;
    if (_refreshTimer >= 30) {
        _refreshTimer = 0;
        requestInventories(state, connection);
    }
}

void TeamList::draw(const game_state &state) const
{
    if (!_open)
        return;
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
    if (_selectedTeam.empty())
        drawTeamPanel(state);
    else
        drawPlayerPanel(state);
}
