/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

//#include "zappy.hpp"
#include <stdio.h>
#include <raylib.h>

int main()
{
    InitWindow(1920, 1080, "ZAPPY GUI");
    SetTargetFPS(60);
    Texture2D background = LoadTexture("background.jpg");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background, 0, 0, WHITE);
        DrawText("Welcome on ZAPPY GUI", 100, 100, 50, LIGHTGRAY);
        EndDrawing();
    }
    UnloadTexture(background);
    CloseWindow();
    return 0;
}
