/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** inventory
*/

#include "commands.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void cmd_inventory(server_t *s, client_t *c)
{
    player_t *p = c->player;
    char buf[256];

    if (!p)
        return;
    snprintf(buf, sizeof(buf),
        "[food %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras %d, thystame %d]\n",
        p->inventory[FOOD],
        p->inventory[LINEMATE],
        p->inventory[DERAUMERE],
        p->inventory[SIBUR],
        p->inventory[MENDIANE],
        p->inventory[PHIRAS],
        p->inventory[THYSTAME]);
    gui_event_pin(s, p);
    write(c->fd, buf, strlen(buf));
}
