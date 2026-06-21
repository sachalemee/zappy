/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** fork
*/

#include <stdlib.h>
#include <unistd.h>
#include "commands.h"

int pre_cmd_fork(server_t *s, client_t *c)
{
    (void)s;
    if (!c->player)
        return -1;
    gui_event_pfk(s, c->player);
    return 0;
}

void cmd_fork(server_t *s, client_t *c)
{
    if (!c->player || !c->player->team)
        return;

    egg_t *egg = spawn_egg(s, c);
    if (!egg)
        return;
    c->player->team->free_slots++;
    gui_event_enw(s, egg->id, c->player);
    write(c->fd, "ok\n", 3);
}
