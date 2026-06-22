/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** connection_manager
*/

#include "network/connection_manager.hpp"
#include <iostream>
#include <vector>

bool connection_manager::connect(const std::string &host, int port)
{
    _isConnected = false;
    return _net.connectTo(host, port);
}

void connection_manager::disconnect()
{
    _net.disconnect();
    _isConnected = false;
}

void connection_manager::handleLine(const std::string &line, game_state &state)
{
    if (_isConnected) {
        std::cout << "[SERVER] " << line << std::endl;
        state.handleLine(line);
    } else if (line == "WELCOME") {
        _net.sendLine("GRAPHIC\n");
        _isConnected = true;
    }
}

void connection_manager::update(game_state &state)
{
    if (_isConnected && !_net.isConnected()) {
        state.reset();
        _isConnected = false;
        return;
    }

    std::vector<std::string> lines = _net.poll();

    for (std::size_t i = 0; i < lines.size(); i++)
        handleLine(lines[i], state);
}
