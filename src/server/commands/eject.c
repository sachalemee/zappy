/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** eject
*/

#include "commands.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static void move_player(server_t *s, player_t *p, int dir)
{
    if (dir == 1)
        p->y = (p->y - 1 + s->height) % s->height;
    else if (dir == 2)
        p->x = (p->x + 1) % s->width;
    else if (dir == 3)
        p->y = (p->y + 1) % s->height;
    else if (dir == 4)
        p->x = (p->x - 1 + s->width) % s->width;
    gui_event_ppo(s, p);
}

static int get_eject_k(int pusher_ori, int victim_ori)
{
    int origin = (pusher_ori + 1) % 4 + 1;
    int diff = origin - victim_ori;
    int k_map[4] = {1, 7, 5, 3};

    if (diff < 0)
        diff += 4;
    return k_map[diff];
}

static int push_players(server_t *s, player_t *p)
{
    int pushed = 0;
    char buf[16];

    for (int i = 0; i < s->nb_clients; i++) {
        client_t *o = s->clients[i];
        if (!o || o->type != CLIENT_AI || !o->player || o->player == p)
            continue;
        if (o->player->x == p->x && o->player->y == p->y) {
            move_player(s, o->player, p->orientation);
            snprintf(buf, sizeof(buf), "eject: %d\n",
                get_eject_k(p->orientation, o->player->orientation));
            write(o->fd, buf, strlen(buf));
            pushed++;
        }
    }
    return pushed;
}

static void destroy_eggs_on_tile(server_t *s, player_t *p)
{
    egg_t *e;

    for (int i = 0; i < s->nb_eggs; i++) {
        e = s->eggs[i];
        if (e && e->x == p->x && e->y == p->y) {
            destroy_egg(s, e->id);
            i--;
        }
    }
}

void cmd_eject(server_t *s, client_t *c)
{
    int pushed;

    if (!c->player) {
        write(c->fd, "ko\n", 3);
        return;
    }
    pushed = push_players(s, c->player);
    destroy_eggs_on_tile(s, c->player);
    if (pushed > 0) {
        gui_event_pex(s, c->player->id);
        write(c->fd, "ok\n", 3);
    } else {
        write(c->fd, "ko\n", 3);
    }
}
