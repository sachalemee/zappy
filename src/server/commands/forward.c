/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** forward
*/

#include "commands.h"
#include <unistd.h>

void cmd_forward(server_t *s, client_t *c)
{
    (void)s;

    if (c->player)
        c->player->y++;
    write(c->fd, "ok\n", 3);
}
