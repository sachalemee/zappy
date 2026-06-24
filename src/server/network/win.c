/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** win
*/

#include "zappy.h"

int check_win(server_t *s)
{
    int count;

    if (!s || !s->teams || !s->clients)
        return 0;
    for (int i = 0; i < s->nb_teams; i++) {
        count = 0;
        for (int j = 0; j < s->nb_clients; j++) {
            if (!s->clients[j])
                continue;
            if (s->clients[j]->type != CLIENT_AI)
                continue;
            if (!s->clients[j]->player)
                continue;
            if (s->clients[j]->player->team != &s->teams[i])
                continue;
            if (s->clients[j]->player->level >= 8)
                count++;
        }
        if (count >= 6) {
            gui_event_seg(s, s->teams[i].name);
            return 1;
        }
    }
    return 0;
}
