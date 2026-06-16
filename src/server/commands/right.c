/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** right
*/

#include "commands.h"
#include <unistd.h>

void cmd_right(server_t *s, client_t *c)
{
    player_t *p = c->player;

    if (!p)
        return;
    p->orientation = (p->orientation % 4) + 1;
    gui_event_ppo(s, p);
    write(c->fd, "ok\n", 3);
}
