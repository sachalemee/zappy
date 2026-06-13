/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** inventory
*/

#include "commands.h"
#include <unistd.h>

void cmd_inventory(server_t *s, client_t *c)
{
    (void)s;
    write(c->fd, "[food 10]\n", 11);
}
