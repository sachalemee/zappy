/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** timeout
*/

#include "zappy.h"
#include <stddef.h>

int calculate_timeout(server_t *s)
{
    long long current = get_current_time();
    long long min_delay = -1;
    long long delay;
    client_t *c;

    for (int i = 0; i < s->nb_clients; i++) {
        c = s->clients[i];
        if (!c || c->state != STATE_IN_GAME)
            continue;
        if (c->action_end_time > 0) {
            delay = c->action_end_time - current;
            if (delay < 0)
                delay = 0;
            if (min_delay == -1 || delay < min_delay)
                min_delay = delay;
        }
        if (c->type == CLIENT_AI && c->player) {
            delay = c->player->next_meal_time - current;
            if (delay < 0)
                delay = 0;
            if (min_delay == -1 || delay < min_delay)
                min_delay = delay;
        }
    }
    return (int)min_delay;
}