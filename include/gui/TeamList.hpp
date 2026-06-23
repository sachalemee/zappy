/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamList
*/

#ifndef TEAMLIST_HPP_
#define TEAMLIST_HPP_

#include <raylib.h>
#include "game/gamestate.hpp"
#include "network/connection_manager.hpp"

class TeamList {
public:
    TeamList();

    void update(const game_state &state, connection_manager &connection);
    void draw(const game_state &state) const;

private:
    void drawTitle(float panelX, float panelY, float panelWidth,
            const char *title) const;
    void drawTeamRow(float x, float y, float width, const std::string &name,
            int count) const;
    void drawPlayerRow(float x, float y, float width,
            const Player &player) const;
    void drawTeamPanel(const game_state &state) const;
    void drawPlayerPanel(const game_state &state) const;
    void drawBackButton(Rectangle box) const;
    int countPlayersInTeam(const std::vector<Player> &players,
            const std::string &team) const;
    Rectangle teamPanelRect(const game_state &state) const;
    Rectangle playerPanelRect(const game_state &state) const;
    Rectangle teamRowRect(const game_state &state, size_t index) const;
    Rectangle backButtonRect(const game_state &state) const;
    void requestInventories(const game_state &state,
            connection_manager &connection);

    bool _open;
    std::string _selectedTeam;
    int _refreshTimer;
};

#endif /* TEAMLIST_HPP_ */
