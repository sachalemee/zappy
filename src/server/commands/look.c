/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** look
*/

#include "commands.h"
#include <unistd.h>

void cmd_look(client_t *c)
{
    write(c->fd, "[player]\n", 9);
}
