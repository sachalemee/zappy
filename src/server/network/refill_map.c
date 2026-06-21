/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** refill_map
*/

#include <stdlib.h>
#include "zappy.h"

static int count_resource(server_t *s, resource_t res)
{
    int total = 0;

    for (int y = 0; y < s->height; y++) {
        for (int x = 0; x < s->width; x++) {
            total += s->map.tiles[y][x].resources[res];
        }
    }
    return total;
}

static void spawn_resource(server_t *s, resource_t res, float density)
{
    int target = (int)(s->width * s->height * density);
    int current = count_resource(s, res);
    int missing = target - current;
    int x = 0;
    int y = 0;

    if (missing <= 0)
        return;
    for (int i = 0; i < missing; i++) {
        x = rand() % s->width;
        y = rand() % s->height;
        s->map.tiles[y][x].resources[res]++;
        gui_send_bct(s, x, y);
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
