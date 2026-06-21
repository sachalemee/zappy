/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** eject
*/

#include "commands.h"
#include <unistd.h>

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

static int push_players(server_t *s, player_t *p)
{
    int pushed = 0;
    client_t *o;

    for (int i = 0; i < s->nb_clients; i++) {
        o = s->clients[i];
        if (!o || o->type != CLIENT_AI || !o->player || o->player == p)
            continue;
        if (o->player->x == p->x && o->player->y == p->y) {
            move_player(s, o->player, p->orientation);
            write(o->fd, "eject: 1\n", 9);
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
