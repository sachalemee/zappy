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

    for (int i = 0; i < s->nb_clients; i++) {
        if (!s->clients[i] || s->clients[i]->state != STATE_IN_GAME)
            continue;
        if (s->clients[i]->action_end_time > 0) {
            delay = s->clients[i]->action_end_time - current;
            if (delay < 0)
                delay = 0;
            if (min_delay == -1 || delay < min_delay)
                min_delay = delay;
        }
    }
    return (int)min_delay;
}
