/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gamecamera
*/

#ifndef GAMECAMERA_HPP_
#define GAMECAMERA_HPP_

#include <raylib.h>

class game_camera {
public:
    game_camera();
    void update(float sensitivity);
    const Camera3D &get() const { return _cam; }
    void zoomIn();
    void zoomOut();

private:
    void handleMovement();
    void handleRotation(float sensitivity);
    void handleWheelZoom();
    void toggleLock();

    Camera3D _cam;
    bool _locked = false;
};

#endif /* !GAMECAMERA_HPP_ */
