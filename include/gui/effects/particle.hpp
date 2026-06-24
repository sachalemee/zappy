/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-11
** File description:
** particle
*/

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <raylib.h>

class Particle {
public:
    Particle(Vector3 position, Vector3 velocity, Color color, float size,
        float life);

    void update(float dt);
    bool isAlive() const;
    const Vector3 &getPosition() const;
    const Color &getColor() const;
    float getSize() const;
    float getLifeRatio() const;

private:
    Vector3 _position;
    Vector3 _velocity;
    Color _color;
    float _size;
    float _life;
    float _maxLife;
};

#endif /* !PARTICLE_HPP_ */
