/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-11
** File description:
** particle
*/

#include "effects/particle.hpp"

static constexpr float GRAVITY = 1.5f;

Particle::Particle(Vector3 position, Vector3 velocity, Color color,
    float size, float life)
    : _position(position)
    , _velocity(velocity)
    , _color(color)
    , _size(size)
    , _life(life)
    , _maxLife(life)
{
}

void Particle::update(float dt)
{
    _position.x += _velocity.x * dt;
    _position.y += _velocity.y * dt;
    _position.z += _velocity.z * dt;
    _velocity.y -= GRAVITY * dt;
    _life -= dt;
}

bool Particle::isAlive() const
{
    return _life > 0.0f;
}

const Vector3 &Particle::getPosition() const
{
    return _position;
}

const Color &Particle::getColor() const
{
    return _color;
}

float Particle::getSize() const
{
    return _size;
}

float Particle::getLifeRatio() const
{
    return _life / _maxLife;
}
