/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** commands
*/

#include <unistd.h>
#include "zappy.h"

void cmd_forward(server_t *s, client_t *c)
{
    (void)s;
    write(c->fd, "ok\n", 3);
}

void cmd_right(server_t *s, client_t *c)
{
    (void)s;
    write(c->fd, "ok\n", 3);
}

void cmd_left(server_t *s, client_t *c)
{
    (void)s;
    write(c->fd, "ok\n", 3);
}
