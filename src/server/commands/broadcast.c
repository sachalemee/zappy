/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** broadcast
*/

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "commands.h"

static int get_shortest(int v1, int v2, int max)
{
    int diff = v1 - v2;

    if (diff > max / 2)
        diff -= max;
    else if (diff < -(max / 2))
        diff += max;
    return diff;
}

static int get_base_dir(int dx, int dy)
{
    if (dx == 0 && dy < 0) return 1;
    if (dx < 0 && dy < 0) return 2;
    if (dx < 0 && dy == 0) return 3;
    if (dx < 0 && dy > 0) return 4;
    if (dx == 0 && dy > 0) return 5;
    if (dx > 0 && dy > 0) return 6;
    if (dx > 0 && dy == 0) return 7;
    if (dx > 0 && dy < 0) return 8;
    return 0;
}

static int get_relative_dir(int base, int orientation)
{
    if (base == 0)
        return 0;
    return (base - 1 + (orientation - 1) * 2) % 8 + 1;
}

static void send_msg_to_client(server_t *s, player_t *p, client_t *o, char *m)
{
    int dx = get_shortest(p->x, o->player->x, s->width);
    int dy = get_shortest(p->y, o->player->y, s->height);
    int base = get_base_dir(dx, dy);
    int dir = get_relative_dir(base, o->player->orientation);
    char buf[MAX_CMD_LEN];

    snprintf(buf, sizeof(buf), "message %d, %s\n", dir, m);
    write(o->fd, buf, strlen(buf));
}

void cmd_broadcast(server_t *s, client_t *c)
{
    char *msg = strchr(c->cmds[0], ' ');

    if (!msg) {
        write(c->fd, "ko\n", 3);
        return;
    }
    msg++;
    gui_event_pbc(s, c->player, msg);
    for (int i = 0; i < s->nb_clients; i++) {
        if (s->clients[i] && s->clients[i]->type == CLIENT_AI &&
            s->clients[i]->state == STATE_IN_GAME && s->clients[i] != c) {
            send_msg_to_client(s, c->player, s->clients[i], msg);
        }
    }
    write(c->fd, "ok\n", 3);
}
