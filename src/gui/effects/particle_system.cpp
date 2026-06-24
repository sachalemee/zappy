/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-11
** File description:
** particle_system
*/

#include "effects/particle_system.hpp"
#include <cstdlib>

static constexpr const char *TEXTURE_PATH =
    "src/gui/models/textures/particle.png";
static constexpr float PARTICLE_LIFE = 0.8f;
static constexpr float PARTICLE_SIZE = 0.3f;

static float randomFloat(float min, float max)
{
    return min + (max - min) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

ParticleSystem::ParticleSystem()
    : _texture(LoadTexture(TEXTURE_PATH))
{
}

ParticleSystem::~ParticleSystem()
{
    UnloadTexture(_texture);
}

void ParticleSystem::spawn(Vector3 origin, Color color, int count)
{
    for (int i = 0; i < count; i++) {
        Vector3 velocity = {randomFloat(-1.5f, 1.5f), randomFloat(1.0f, 3.0f),
            randomFloat(-1.5f, 1.5f)};

        _particles.emplace_back(origin, velocity, color, PARTICLE_SIZE,
            PARTICLE_LIFE);
    }
}

void ParticleSystem::update(float dt)
{
    for (Particle &p : _particles)
        p.update(dt);
    std::erase_if(_particles, [](const Particle &p) {
        return !p.isAlive();
    });
}

void ParticleSystem::draw(const Camera3D &cam) const
{
    BeginBlendMode(BLEND_ALPHA);
    for (const Particle &p : _particles) {
        Color c = p.getColor();

        c.a = static_cast<unsigned char>(255.0f * p.getLifeRatio());
        DrawBillboard(cam, _texture, p.getPosition(), p.getSize(), c);
    }
    EndBlendMode();
}