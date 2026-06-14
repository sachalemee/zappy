/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** famine
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "zappy.h"

static void delete_player(server_t *s, player_t *p)
{
    for (int i = 0; i < s->nb_players; i++) {
        if (s->players[i] == p) {
            free(p);
            for (int j = i; j < s->nb_players - 1; j++)
                s->players[j] = s->players[j + 1];
            s->players[s->nb_players - 1] = NULL;
            s->nb_players--;
            return;
        }
    }
}

static void remove_dead_client(server_t *s, int i)
{
    client_t *c = s->clients[i];

    if (c->fd != -1)
        close(c->fd);
    if (c->player)
        delete_player(s, c->player);
    free(c);
    for (int j = i; j < s->nb_clients - 1; j++)
        s->clients[j] = s->clients[j + 1];
    s->clients[s->nb_clients - 1] = NULL;
    s->nb_clients--;
}

static void kill_player(server_t *s, int idx)
{
    client_t *c = s->clients[idx];
    char buf[64];

    write(c->fd, "dead\n", 5);
    snprintf(buf, sizeof(buf), "pdi %d\n", c->player->id);
    send_to_gui(s, buf);
    remove_dead_client(s, idx);
}

void handle_famine(server_t *s)
{
    long long current = get_current_time();
    client_t *c;

    for (int i = 0; i < s->nb_clients; i++) {
        c = s->clients[i];
        if (!c || c->state != STATE_IN_GAME || c->type != CLIENT_AI)
            continue;
        if (current >= c->player->next_meal_time) {
            c->player->inventory[FOOD]--;
            if (c->player->inventory[FOOD] <= 0) {
                kill_player(s, i);
                i--;
            } else {
                c->player->next_meal_time += get_action_duration(s->freq, 126);
            }
        }
    }
}
