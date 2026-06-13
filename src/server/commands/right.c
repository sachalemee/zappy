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
    (void)s;
    if (c->player)
        c->player->orientation = (c->player->orientation % 4) + 1;
    write(c->fd, "ok\n", 3);
}
