/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** inventory
*/

#include "commands.h"
#include <unistd.h>

void cmd_inventory(client_t *c)
{
    write(c->fd, "[food 10]\n", 11);
}
