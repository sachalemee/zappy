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

    for (int i = 0; i < s->nb_teams; i++) {
        count = 0;
        for (int j = 0; j < s->nb_clients; j++) {
            if (s->clients[j] && s->clients[j]->type == CLIENT_AI &&
                s->clients[j]->player &&
                s->clients[j]->player->team == &s->teams[i] &&
                s->clients[j]->player->level >= 8)
                count++;
        }
        if (count >= 6) {
            gui_event_seg(s, s->teams[i].name);
            return 1;
        }
    }
    return 0;
}
