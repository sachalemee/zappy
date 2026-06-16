/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** protocol
*/

#include <stdio.h>
#include <unistd.h>
#include "zappy.h"

void gui_send_bct(server_t *s, int x, int y)
{
    char buf[256];
    tile_t *tile = &s->map.tiles[y][x];

    snprintf(buf, sizeof(buf),
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        tile->resources[FOOD],
        tile->resources[LINEMATE],
        tile->resources[DERAUMERE],
        tile->resources[SIBUR],
        tile->resources[MENDIANE],
        tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    send_to_gui(s, buf);
}

void gui_send_mct(server_t *s)
{
    for (int y = 0; y < s->height; y++) {
        for (int x = 0; x < s->width; x++) {
            gui_send_bct(s, x, y);
        }
    }
}

void gui_send_ppo(server_t *s, player_t *p)
{
    char buffer[128];

    snprintf(buffer, sizeof(buffer), "ppo #%d %d %d %d\n", p->id, p->x, p->y, p->orientation);
    send_to_gui(s, buffer);
}
