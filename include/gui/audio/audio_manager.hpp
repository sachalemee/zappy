/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-11
** File description:
** audio_manager
*/

#ifndef AUDIO_MANAGER_HPP_
#define AUDIO_MANAGER_HPP_

#include <raylib.h>
#include <unordered_map>
#include <string>

enum class SoundId {
    Death,
    Drop,
    EggCrack,
    EggLay,
    Food,
    LevelUp,
    PickUp,
    Spawn,
    Incantation,
    IncantationFail,
};

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    AudioManager(const AudioManager &) = delete;
    AudioManager &operator=(const AudioManager &) = delete;

    void play(SoundId id);
    void stopAll();
    void setVolume(float v);
    float getVolume() const;
    void setMuted(bool m);
    bool isMuted() const;
    void toggleMute();

private:
    void load(SoundId id, const std::string &path);

    std::unordered_map<SoundId, Sound> _sounds;
    float _volume;
    bool _muted;
};

#endif /* !AUDIO_MANAGER_HPP_ */
