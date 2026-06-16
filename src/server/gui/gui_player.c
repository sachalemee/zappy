/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gui_player
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

static player_t *find_player(server_t *s, int id)
{
    for (int i = 0; i < s->nb_players; i++) {
        if (s->players[i] && s->players[i]->id == id)
            return s->players[i];
    }
    return NULL;
}

static void gui_pin(server_t *s, client_t *c, int id)
{
    player_t *p = find_player(s, id);
    char buf[256];

    if (!p)
        return;

    snprintf(buf, sizeof(buf),
        "pin #%d %d %d %d %d %d %d %d\n",
        p->id,
        p->inventory[FOOD],
        p->inventory[LINEMATE],
        p->inventory[DERAUMERE],
        p->inventory[SIBUR],
        p->inventory[MENDIANE],
        p->inventory[PHIRAS],
        p->inventory[THYSTAME]);
    write(c->fd, buf, strlen(buf));
}

static void gui_plv(server_t *s, client_t *c, int id)
{
    player_t *p = find_player(s, id);
    char buf[128];

    if (!p)
        return;
    snprintf(buf, sizeof(buf), "plv #%d %d\n", p->id, p->level);
    write(c->fd, buf, strlen(buf));
}

static void gui_ppo(server_t *s, client_t *c, int id)
{
    player_t *p = find_player(s, id);
    char buf[128];

    if (!p)
        return;
    snprintf(buf, sizeof(buf), "ppo #%d %d %d %d\n", p->id, p->x, p->y, p->orientation);
    write(c->fd, buf, strlen(buf));
}

void handle_gui_player_command(server_t *s, client_t *c)
{
    char *cmd = c->cmds[0];
    int id;

    if (strncmp(cmd, "pin", 3) == 0) {
        if (sscanf(cmd, "pin #%d", &id) == 1)
            gui_pin(s, c, id);
        return;
    }
    if (strncmp(cmd, "plv", 3) == 0) {
        if (sscanf(cmd, "plv #%d", &id) == 1)
            gui_plv(s, c, id);
        return;
    }
    if (strncmp(cmd, "ppo", 3) == 0) {
        if (sscanf(cmd, "ppo #%d", &id) == 1)
            gui_ppo(s, c, id);
        return;
    }
    write(c->fd, "suc\n", 4);
}
