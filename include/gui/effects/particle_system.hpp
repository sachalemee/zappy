/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-11
** File description:
** particle_system
*/

#ifndef PARTICLE_SYSTEM_HPP_
#define PARTICLE_SYSTEM_HPP_

#include "effects/particle.hpp"
#include <vector>

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    ParticleSystem(const ParticleSystem &) = delete;
    ParticleSystem &operator=(const ParticleSystem &) = delete;

    void spawn(Vector3 origin, Color color, int count);
    void update(float dt);
    void draw(const Camera3D &cam) const;

private:
    std::vector<Particle> _particles;
    Texture2D _texture;
};

#endif /* !PARTICLE_SYSTEM_HPP_ */