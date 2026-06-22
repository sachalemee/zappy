/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** connection_manager
*/

#include "network/connection_manager.hpp"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

static constexpr int FOOD_RESOURCE = 0;

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

SoundId connection_manager::resourceSound(std::istringstream &args,
    SoundId fallback) const
{
    std::string id;
    int res = -1;

    args >> id >> res;
    return res == FOOD_RESOURCE ? SoundId::Food : fallback;
}

bool connection_manager::incantationFailed(std::istringstream &args) const
{
    int x = 0;
    int y = 0;
    int result = 0;

    args >> x >> y >> result;
    return result == 0;
}

void connection_manager::playEventSound(const std::string &line,
    AudioManager &audio)
{
    static const std::unordered_map<std::string, SoundId> SIMPLE_SOUNDS = {
        {"pnw", SoundId::Spawn},
        {"pdi", SoundId::Death},
        {"pic", SoundId::Incantation},
        {"plv", SoundId::LevelUp},
        {"pfk", SoundId::EggLay},
        {"enw", SoundId::EggLay},
        {"ebo", SoundId::EggCrack},
        {"edi", SoundId::Death},
    };

    if (line.size() < 3)
        return;
    std::string cmd = line.substr(0, 3);
    std::istringstream args(line.substr(3));
    auto it = SIMPLE_SOUNDS.find(cmd);

    if (it != SIMPLE_SOUNDS.end())
        audio.play(it->second);
    else if (cmd == "pie" && incantationFailed(args))
        audio.play(SoundId::IncantationFail);
    else if (cmd == "pdr")
        audio.play(resourceSound(args, SoundId::Drop));
    else if (cmd == "pgt")
        audio.play(resourceSound(args, SoundId::PickUp));
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
