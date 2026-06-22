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

void connection_manager::playEventSound(const std::string &line,
    AudioManager &audio)
{
    if (line.size() < 3)
        return;
    std::string cmd = line.substr(0, 3);

    if (cmd == "pnw")
        audio.play(SoundId::Spawn);
    else if (cmd == "pdi")
        audio.play(SoundId::Death);
    else if (cmd == "pic")
        audio.play(SoundId::Incantation);
    else if (cmd == "pie") {
        if (line.size() >= 5 && line.find(" 1") != std::string::npos)
            audio.play(SoundId::LevelUp);
    } else if (cmd == "pdr")
        audio.play(SoundId::Drop);
    else if (cmd == "pgt")
        audio.play(SoundId::PickUp);
    else if (cmd == "pfk")
        audio.play(SoundId::EggLay);
    else if (cmd == "enw")
        audio.play(SoundId::EggLay);
    else if (cmd == "ebo")
        audio.play(SoundId::EggCrack);
    else if (cmd == "edi")
        audio.play(SoundId::Death);
    else if (cmd == "seg")
        audio.play(SoundId::Transition);
}

void connection_manager::handleLine(const std::string &line,
    game_state &state, AudioManager &audio)
{
    if (_isConnected) {
        std::cout << "[SERVER] " << line << std::endl;
        playEventSound(line, audio);
        state.handleLine(line);
    } else if (line == "WELCOME") {
        _net.sendLine("GRAPHIC\n");
        _isConnected = true;
    }
}

void connection_manager::update(game_state &state, AudioManager &audio)
{
    if (_isConnected && !_net.isConnected()) {
        state.reset();
        _isConnected = false;
        return;
    }

    std::vector<std::string> lines = _net.poll();
    for (std::size_t i = 0; i < lines.size(); i++)
        handleLine(lines[i], state, audio);
}
