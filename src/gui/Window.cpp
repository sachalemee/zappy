/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Window
*/

#include "Window.hpp"

Window::Window(int width, int height, const std::string &title, int fps)
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(fps);
}

Window::~Window()
{
    CloseWindow();
}

bool Window::shouldClose() const
{
    return WindowShouldClose();
}

void Window::beginFrame(Color bg)
{
    BeginDrawing();
    ClearBackground(bg);
}

void Window::endFrame()
{
    EndDrawing();
}
