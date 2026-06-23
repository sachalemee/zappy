/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamListNetwork
*/

#include "TeamList.hpp"

void TeamList::requestInventories(const game_state &state,
        connection_manager &connection)
{
    const auto &players = state.players();

    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].team == _selectedTeam)
            connection.sendCommand("pin #" + std::to_string(players[i].id)
                + "\n");
    }
}
