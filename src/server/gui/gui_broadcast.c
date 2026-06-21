/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gui_broadcast
*/

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "zappy.h"

void send_to_gui(server_t *s, const char *msg)
{
    int len = strlen(msg);

    for (int i = 0; i < s->nb_clients; i++) {
        if (s->clients[i] && 
            s->clients[i]->type == CLIENT_GUI && 
            s->clients[i]->state == STATE_IN_GAME) {
            write(s->clients[i]->fd, msg, len);
        }
    }
}

void gui_send_initial_data(server_t *s, int fd)
{
    char buf[128];

    for (int i = 0; i < s->nb_players; i++) {
        if (!s->players[i]) continue;
        snprintf(buf, sizeof(buf), "pnw #%d %d %d %d %d %s\n", s->players[i]->id,
            s->players[i]->x, s->players[i]->y, s->players[i]->orientation,
            s->players[i]->level, s->players[i]->team->name);
        write(fd, buf, strlen(buf));
    }
    for (int i = 0; i < s->nb_eggs; i++) {
        if (!s->eggs[i]) continue;
        snprintf(buf, sizeof(buf), "enw #%d #%d %d %d\n", s->eggs[i]->id,
            s->eggs[i]->player_id, s->eggs[i]->x, s->eggs[i]->y);
        write(fd, buf, strlen(buf));
        if (s->eggs[i]->hatched) {
            snprintf(buf, sizeof(buf), "eht #%d\n", s->eggs[i]->id);
            write(fd, buf, strlen(buf));
        }
    }
}