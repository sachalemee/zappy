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
    (void)s;
    if (c->player)
        c->player->orientation--;
    if (c->player && c->player->orientation <= 0)
        c->player->orientation = 4;
    write(c->fd, "ok\n", 3);
}
