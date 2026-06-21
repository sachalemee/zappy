/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** incantation
*/

#include "commands.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static const int REQ[7][7] = {
    {0, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0},
    {0, 2, 0, 1, 0, 2, 0},
    {0, 1, 1, 2, 0, 1, 0},
    {0, 1, 2, 1, 3, 0, 0},
    {0, 1, 2, 3, 0, 1, 0},
    {0, 2, 2, 2, 2, 2, 1}
};

static const int P_REQ[7] = {1, 2, 2, 4, 4, 6, 6};

static int check_stones(server_t *s, player_t *p)
{
    int lvl = p->level - 1;
    tile_t *t = &s->map.tiles[p->y][p->x];

    for (int i = 1; i < RESOURCE_MAX; i++) {
        if (t->resources[i] < REQ[lvl][i])
            return -1;
    }
    return 0;
}

static int count_players(server_t *s, player_t *p)
{
    int count = 0;
    client_t *c;

    for (int i = 0; i < s->nb_clients; i++) {
        c = s->clients[i];
        if (c && c->type == CLIENT_AI && c->player &&
            c->player->x == p->x && c->player->y == p->y &&
            c->player->level == p->level) {
            count++;
        }
    }
    return count;
}

int pre_cmd_incantation(server_t *s, client_t *c)
{
    player_t *p = c->player;
    client_t *o;

    if (!p || p->level >= 8 || check_stones(s, p) == -1)
        return -1;
    if (count_players(s, p) != P_REQ[p->level - 1])
        return -1;
    gui_event_pic(s, p);
    for (int i = 0; i < s->nb_clients; i++) {
        o = s->clients[i];
        if (o && o->type == CLIENT_AI && o->player &&
            o->player->x == p->x && o->player->y == p->y &&
            o->player->level == p->level) {
            write(o->fd, "Elevation underway\n", 19);
            if (o != c)
                o->action_end_time = get_current_time() +
                    get_action_duration(s->freq, 300);
        }
    }
    return 0;
}

static void remove_stones(server_t *s, player_t *p)
{
    int lvl = p->level - 1;
    tile_t *t = &s->map.tiles[p->y][p->x];

    for (int i = 1; i < RESOURCE_MAX; i++) {
        t->resources[i] -= REQ[lvl][i];
    }
    gui_event_bct(s, p->x, p->y);
}

static void level_up_players(server_t *s, player_t *p)
{
    int target = p->level;
    client_t *c;
    char buf[64];

    for (int i = 0; i < s->nb_clients; i++) {
        c = s->clients[i];
        if (c && c->type == CLIENT_AI && c->player &&
            c->player->x == p->x && c->player->y == p->y &&
            c->player->level == target) {
            c->player->level++;
            snprintf(buf, sizeof(buf), "Current level: %d\n", c->player->level);
            write(c->fd, buf, strlen(buf));
            gui_event_plv(s, c->player);
        }
    }
}

void cmd_incantation(server_t *s, client_t *c)
{
    player_t *p = c->player;

    if (!p)
        return;
    if (check_stones(s, p) == -1 || count_players(s, p) != P_REQ[p->level - 1]) {
        write(c->fd, "ko\n", 3);
        gui_event_pie(s, p, 0);
        return;
    }
    remove_stones(s, p);
    level_up_players(s, p);
    gui_event_pie(s, p, 1);
}
