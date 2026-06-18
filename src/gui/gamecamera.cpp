/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gamecamera
*/

#include "gamecamera.hpp"

game_camera::game_camera()
{
    _cam.position = {15.0f, 15.0f, 15.0f};
    _cam.target = {5.0f, 0.0f, 5.0f};
    _cam.up = {0.0f, 1.0f, 0.0f};
    _cam.fovy = 45.0f;
    _cam.projection = CAMERA_PERSPECTIVE;
}

void game_camera::toggleLock()
{
    _locked = !_locked;
    if (_locked)
        DisableCursor();
    else
        EnableCursor();
}

    void game_camera::handleMovement()
    {
        Vector3 move = {0.0f, 0.0f, 0.0f};
        float speed = 0.2f;

        if (IsKeyDown(KEY_W))
            move.x += speed;
        if (IsKeyDown(KEY_S))
            move.x -= speed;
        if (IsKeyDown(KEY_D))
            move.y += speed;
        if (IsKeyDown(KEY_A))
            move.y -= speed;
        if (IsKeyDown(KEY_Q))
            move.z += speed;
        if (IsKeyDown(KEY_E))
            move.z -= speed;
        UpdateCameraPro(&_cam, move, {0.0f, 0.0f, 0.0f}, 0.0f);
    }

void game_camera::handleRotation(float sensitivity)
{
    Vector2 delta = GetMouseDelta();
    Vector3 look = {0.0f, 0.0f, 0.0f};

    if (!_locked)
        return;
    look.x = delta.x * sensitivity * 0.001f;
    look.y = delta.y * sensitivity * 0.001f;
    UpdateCameraPro(&_cam, {0.0f, 0.0f, 0.0f}, look, 0.0f);
}

void game_camera::zoomIn()
{
    _cam.fovy -= 5.0f;
    if (_cam.fovy < 10.0f)
        _cam.fovy = 10.0f;
}

void game_camera::zoomOut()
{
    _cam.fovy += 5.0f;
    if (_cam.fovy > 90.0f)
        _cam.fovy = 90.0f;
}

void game_camera::update(float sensitivity)
{
    if (IsKeyPressed(KEY_X))
        toggleLock();
    handleMovement();
    handleRotation(sensitivity);
}
