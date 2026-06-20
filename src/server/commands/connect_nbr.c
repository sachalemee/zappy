/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** connect_nbr
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"

void cmd_connect_nbr(server_t *s, client_t *c)
{
    char buf[32];

    (void)s;
    if (!c || !c->team)
        return;
    snprintf(buf, sizeof(buf), "%d\n", c->team->free_slots);
    write(c->fd, buf, strlen(buf));
}
