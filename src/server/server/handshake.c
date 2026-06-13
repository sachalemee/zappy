/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** handshake
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "zappy.h"

static void pop_command(client_t *c)
{
    if (c->nb_cmds <= 0)
        return;
    for (int i = 0; i < c->nb_cmds - 1; i++) {
        strncpy(c->cmds[i], c->cmds[i + 1], MAX_CMD_LEN);
    }
    c->nb_cmds--;
}

static void assign_graphic(client_t *c)
{
    c->state = STATE_IN_GAME;
    pop_command(c);
}

static void assign_team(client_t *c, team_t *t)
{
    char buf[128];
    int len;

    if (t->free_slots <= 0) {
        c->fd = -1;
        return;
    }
    t->free_slots--;
    c->team = t;
    c->state = STATE_IN_GAME;
    len = snprintf(buf, sizeof(buf), "%d\n%d %d\n", t->free_slots,
        c->server->width, c->server->height);
    write(c->fd, buf, len);
    pop_command(c);
}

void handle_handshake(client_t *c)
{
    if (c->nb_cmds == 0)
        return;
    if (strcmp(c->cmds[0], "GRAPHIC") == 0) {
        assign_graphic(c);
        return;
    }
    for (int i = 0; i < c->server->nb_teams; i++) {
        if (strcmp(c->cmds[0], c->server->teams[i].name) == 0) {
            assign_team(c, &c->server->teams[i]);
            return;
        }
    }
    c->fd = -1;
}