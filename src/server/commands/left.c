/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** left
*/

#include "commands.h"
#include <unistd.h>

void cmd_left(server_t *s, client_t *c)
{
    player_t *p = c->player;

    if (!p)
        return;
    p->orientation--;
    if (p->orientation <= 0)
        p->orientation = 4;
    gui_event_ppo(s, p);
    write(c->fd, "ok\n", 3);
}
