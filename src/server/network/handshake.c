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

static void assign_graphic(client_t *c)
{
    char buf[256];
    int len;
    player_t *p;

    c->state = STATE_IN_GAME;
    c->type = CLIENT_GUI;
    len = snprintf(buf, sizeof(buf),
        "msz %d %d\nsgt %d\n",
        c->server->width,
        c->server->height,
        c->server->freq);
    write(c->fd, buf, len);
    for (int i = 0; i < c->server->nb_teams; i++) {
        len = snprintf(buf, sizeof(buf),
            "tna %s\n",
            c->server->teams[i].name);
        write(c->fd, buf, len);
    }
    gui_send_mct(c->server);
    for (int i = 0; i < c->server->nb_players; i++) {
        p = c->server->players[i];
        len = snprintf(buf, sizeof(buf),
            "pnw #%d %d %d %d %d %s\n",
            p->id,
            p->x,
            p->y,
            p->orientation,
            p->level,
            p->team->name);
        send_to_gui(c->server, buf);
    }
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
    c->player = create_player(c->server, t);
    if (!c->player) {
        c->fd = -1;
        return;
    }
    c->player->fd = c->fd;
    c->player->next_meal_time = get_current_time() + 
        get_action_duration(c->server->freq, 126);
    add_player(c->server, c->player);
    len = snprintf(buf, sizeof(buf), "%d\n%d %d\n", t->free_slots,
        c->server->width, c->server->height);
    write(c->fd, buf, len);
    len = snprintf(buf, sizeof(buf), "pnw #%d %d %d %d %d %s\n",
        c->player->id, c->player->x, c->player->y,
        c->player->orientation, c->player->level, t->name);
    send_to_gui(c->server, buf);
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
