/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Window
*/

#pragma once
#include <string>
#include <raylib.h>

class Window {
public:
    Window(int width, int height, const std::string &title, int fps = 60);
    ~Window();
    bool shouldClose() const;
    void beginFrame(Color bg = BLACK);
    void endFrame();
};
