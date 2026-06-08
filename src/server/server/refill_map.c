/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** refill_map
*/

#include <stdlib.h>
#include "zappy.h"

static void spawn_resource(server_t *s, resource_t res, float density)
{
    int total = (int)(s->width * s->height * density);
    int x;
    int y;

    for (int i = 0; i < total; i++) {
        x = rand() % s->width;
        y = rand() % s->height;
        s->map.tiles[y][x].resources[res]++;
    }
}

void refill_map(server_t *s)
{
    spawn_resource(s, FOOD, 0.5);
    spawn_resource(s, LINEMATE, 0.3);
    spawn_resource(s, DERAUMERE, 0.15);
    spawn_resource(s, SIBUR, 0.1);
    spawn_resource(s, MENDIANE, 0.1);
    spawn_resource(s, PHIRAS, 0.08);
    spawn_resource(s, THYSTAME, 0.05);
}
