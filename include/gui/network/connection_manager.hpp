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
#include "audio/audio_manager.hpp"
#include "effects/particle_system.hpp"
#include <sstream>
#include <string>

class connection_manager {
public:
    connection_manager() = default;
    bool connect(const std::string &host, int port);
    void disconnect();
    void update(game_state &state, AudioManager &audio,
        ParticleSystem &particles);
    bool sendCommand(const std::string &cmd) { return _net.sendLine(cmd); }
    bool isConnected() const { return _net.isConnected(); }
    bool isReady() const { return _isConnected; }

private:
    void handleLine(const std::string &line, game_state &state,
        AudioManager &audio, ParticleSystem &particles);
    void playEventEffects(const std::string &line, AudioManager &audio,
        ParticleSystem &particles) const;
    bool playEventSound(const std::string &cmd, std::istringstream &args,
        AudioManager &audio) const;
    void spawnIncantationParticles(const std::string &cmd,
        std::istringstream &args, ParticleSystem &particles,
        bool failed) const;
    Vector3 eventPosition(std::istringstream &args) const;
    SoundId resourceSound(std::istringstream &args, SoundId fallback) const;
    bool incantationFailed(std::istringstream &args) const;

    network_client _net;
    bool _isConnected = false;
};

#endif /* CONNECTION_MANAGER_HPP_ */