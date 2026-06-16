/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** look
*/

#include "commands.h"
#include <unistd.h>

void cmd_look(server_t *s, client_t *c)
{
    (void)s;
    write(c->fd, "[player]\n", 9);
}
