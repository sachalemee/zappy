/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** connection_manager
*/

#ifndef CONNECTION_MANAGER_HPP_
#define CONNECTION_MANAGER_HPP_

#include "network/network_client.hpp"
#include "game/gamestate.hpp"
#include <string>

class connection_manager {
public:
    connection_manager() = default;

    bool connect(const std::string &host, int port);
    void disconnect();
    void update(game_state &state);
    bool sendCommand(const std::string &cmd) { return _net.sendLine(cmd); }
    bool isConnected() const { return _net.isConnected(); }
    bool isReady() const { return _isConnected; }

private:
    void handleLine(const std::string &line, game_state &state);

    network_client _net;
    bool _isConnected = false;
};

#endif /* CONNECTION_MANAGER_HPP_ */
