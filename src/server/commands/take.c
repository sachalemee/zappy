/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** take
*/

#include <string.h>
#include <unistd.h>
#include "commands.h"

void cmd_take(server_t *s, client_t *c)
{
    player_t *p = c->player;
    char *arg;
    int res;
    tile_t *tile;

    if (!p)
        return;
    arg = strchr(c->cmds[0], ' ');
    if (!arg) {
        write(c->fd, "ko\n", 3);
        return;
    }
    arg++;
    res = get_resource_type(arg);
    if (res == -1) {
        write(c->fd, "ko\n", 3);
        return;
    }
    tile = &s->map.tiles[p->y][p->x];
    if (tile->resources[res] > 0) {
        tile->resources[res]--;
        p->inventory[res]++;
        gui_event_pgt(s, p, res);
        gui_event_bct(s, p->x, p->y);
        gui_event_pin(s, p);
        write(c->fd, "ok\n", 3);
    } else {
        write(c->fd, "ko\n", 3);
    }
}
