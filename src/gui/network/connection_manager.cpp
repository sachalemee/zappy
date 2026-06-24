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

Vector3 connection_manager::eventPosition(std::istringstream &args) const
{
    int x = 0;
    int y = 0;

    args >> x >> y;
    return {static_cast<float>(x), 0.5f, static_cast<float>(y)};
}

bool connection_manager::playEventSound(const std::string &cmd,
    std::istringstream &args, AudioManager &audio) const
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
    static const std::unordered_map<std::string, SoundId> RESOURCE_SOUNDS = {
        {"pdr", SoundId::Drop},
        {"pgt", SoundId::PickUp},
    };
    auto sound = SIMPLE_SOUNDS.find(cmd);
    auto resource = RESOURCE_SOUNDS.find(cmd);

    if (sound != SIMPLE_SOUNDS.end()) {
        audio.play(sound->second);
        return false;
    }
    if (cmd == "pie") {
        bool failed = incantationFailed(args);

        if (failed)
            audio.play(SoundId::IncantationFail);
        return failed;
    }
    if (resource != RESOURCE_SOUNDS.end())
        audio.play(resourceSound(args, resource->second));
    return false;
}

void connection_manager::spawnIncantationParticles(const std::string &cmd,
    std::istringstream &args, ParticleSystem &particles, bool failed) const
{
    static const Color START_COLOR = {170, 90, 220, 255};
    static const Color SUCCESS_COLOR = {255, 215, 0, 255};

    if (cmd == "pic")
        particles.spawn(eventPosition(args), START_COLOR, 16);
    else if (cmd == "pie" && !failed)
        particles.spawn(eventPosition(args), SUCCESS_COLOR, 16);
}

void connection_manager::playEventEffects(const std::string &line,
    AudioManager &audio, ParticleSystem &particles) const
{
    if (line.size() < 3)
        return;
    std::string cmd = line.substr(0, 3);
    std::istringstream soundArgs(line.substr(3));
    std::istringstream particleArgs(line.substr(3));
    bool failed = playEventSound(cmd, soundArgs, audio);

    spawnIncantationParticles(cmd, particleArgs, particles, failed);
}

void connection_manager::handleLine(const std::string &line,
    game_state &state, AudioManager &audio, ParticleSystem &particles)
{
    if (_isConnected) {
        std::cout << "[SERVER] " << line << std::endl;
        playEventEffects(line, audio, particles);
        state.handleLine(line);
    } else if (line == "WELCOME") {
        _net.sendLine("GRAPHIC\n");
        _isConnected = true;
    }
}

void connection_manager::update(game_state &state, AudioManager &audio,
    ParticleSystem &particles)
{
    if (_isConnected && !_net.isConnected()) {
        state.reset();
        _isConnected = false;
        return;
    }

    std::vector<std::string> lines = _net.poll();
    for (std::size_t i = 0; i < lines.size(); i++)
        handleLine(lines[i], state, audio, particles);
}