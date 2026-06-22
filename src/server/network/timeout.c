/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** timeout
*/

#include "zappy.h"
#include <stddef.h>

static void check_client_timeout(server_t *s, long long curr, long long *min)
{
    long long d;
    client_t *c;

    for (int i = 0; i < s->nb_clients; i++) {
        c = s->clients[i];
        if (!c || c->state != STATE_IN_GAME) continue;
        if (c->action_end_time > 0) {
            d = c->action_end_time - curr;
            if (d < 0) d = 0;
            if (*min == -1 || d < *min) *min = d;
        }
        if (c->type == CLIENT_AI && c->player) {
            d = c->player->next_meal_time - curr;
            if (d < 0) d = 0;
            if (*min == -1 || d < *min) *min = d;
        }
    }
}

static void check_egg_timeout(server_t *s, long long curr, long long *min)
{
    long long d;

    for (int i = 0; i < s->nb_eggs; i++) {
        if (!s->eggs[i]) continue;
        if (!s->eggs[i]->hatched)
            d = s->eggs[i]->hatch_time - curr;
        else
            d = s->eggs[i]->death_time - curr;
        if (d < 0) d = 0;
        if (*min == -1 || d < *min) *min = d;
    }
}

int calculate_timeout(server_t *s)
{
    long long curr = get_current_time();
    long long min = -1;
    long long d;

    check_client_timeout(s, curr, &min);
    check_egg_timeout(s, curr, &min);
    d = s->next_refill_time - curr;
    if (d < 0) d = 0;
    if (min == -1 || d < min) min = d;
    return (int)min;
}
