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

void cmd_eject(server_t *s, client_t *c)
{
    player_t *p = c->player;
    int pushed = 0;

    if (!p) {
        write(c->fd, "ko\n", 3);
        return;
    }
    for (int i = 0; i < s->nb_clients; i++) {
        client_t *o = s->clients[i];
        if (!o || o->type != CLIENT_AI || !o->player)
            continue;
        if (o->player == p)
            continue;
        if (o->player->x == p->x &&
            o->player->y == p->y) {
            move_player(s, o->player, p->orientation);
            write(o->fd, "eject: 1\n", 9);
            pushed++;
        }
    }
    for (int i = 0; i < s->nb_eggs; i++) {
        egg_t *e = s->eggs[i];
        if (e && !e->hatched &&
            e->x == p->x && e->y == p->y) {
            e->hatched = 1;
            gui_event_edi(s, e->id);
        }
    }
    if (pushed > 0)
        write(c->fd, "ok\n", 3);
    else
        write(c->fd, "ko\n", 3);
}
