/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-11
** File description:
** audio_manager
*/

#include "audio/audio_manager.hpp"
#include <algorithm>

static const std::string BASE = "src/gui/audio/sounds/";

AudioManager::AudioManager()
    : _volume(1.0f)
    , _muted(false)
{
    InitAudioDevice();
    load(SoundId::Death, BASE + "death.wav");
    load(SoundId::Drop, BASE + "drop.wav");
    load(SoundId::EggCrack, BASE + "egg_crack.wav");
    load(SoundId::EggLay, BASE + "egg_lay.wav");
    load(SoundId::Food, BASE + "food.wav");
    load(SoundId::LevelUp, BASE + "level_up.wav");
    load(SoundId::PickUp, BASE + "pick_up.wav");
    load(SoundId::Spawn, BASE + "spawn.wav");
    load(SoundId::Incantation, BASE + "incarnation.wav");
    load(SoundId::IncantationFail, BASE + "incantation_fail.wav");
}

AudioManager::~AudioManager()
{
    for (auto &pair : _sounds)
        UnloadSound(pair.second);
    if (_musicLoaded)
        UnloadMusicStream(_music);
    CloseAudioDevice();
}

void AudioManager::load(SoundId id, const std::string &path)
{
    Sound s = LoadSound(path.c_str());
    SetSoundVolume(s, _volume);
    _sounds[id] = s;
}

void AudioManager::play(SoundId id)
{
    if (_muted)
        return;
    auto it = _sounds.find(id);
    if (it != _sounds.end())
        PlaySound(it->second);
}

void AudioManager::stopAll()
{
    for (auto &pair : _sounds)
        StopSound(pair.second);
}

void AudioManager::setVolume(float v)
{
    _volume = std::clamp(v, 0.0f, 1.0f);
    for (auto &pair : _sounds)
        SetSoundVolume(pair.second, _volume);
}

float AudioManager::getVolume() const
{
    return _volume;
}

void AudioManager::setMuted(bool m)
{
    _muted = m;
    if (m)
        stopAll();
}

bool AudioManager::isMuted() const
{
    return _muted;
}

void AudioManager::toggleMute()
{
    setMuted(!_muted);
}

void AudioManager::loadMusic(const std::string &path)
{
    _music = LoadMusicStream(path.c_str());
    _music.looping = true;
    _musicLoaded = true;
}

void AudioManager::updateMusic()
{
    if (_musicLoaded)
        UpdateMusicStream(_music);
}

void AudioManager::playMusic()
{
    if (_musicLoaded)
        PlayMusicStream(_music);
}


void AudioManager::stopMusic()
{
    if (_musicLoaded)
        StopMusicStream(_music);
}

void AudioManager::setMusicVolume(float volume)
{
    _musicVolume = std::clamp(volume, 0.0f, 1.0f);
    if (_musicLoaded)
        SetMusicVolume(_music, _musicVolume);
}

float AudioManager::getMusicVolume() const
{
    return _musicVolume;
}