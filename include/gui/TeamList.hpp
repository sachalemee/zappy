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

class TeamList {
public:
    TeamList();

    void update(const game_state &state);
    void draw(const game_state &state) const;

private:
    void drawPanel(const game_state &state) const;
    void drawTitle(float panelX, float panelY, float panelWidth) const;
    void drawRow(float x, float y, const std::string &name, int count) const;
    int countPlayersInTeam(const std::vector<Player> &players,
            const std::string &team) const;

    bool _open;
};

#endif /* TEAMLIST_HPP_ */
