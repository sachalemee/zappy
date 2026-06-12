/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** left
*/

#include "commands.h"
#include <unistd.h>

void cmd_left(client_t *c)
{
    if (c->player)
        c->player->orientation--;

    if (c->player && c->player->orientation <= 0)
        c->player->orientation = 4;

    write(c->fd, "ok\n", 3);
}
