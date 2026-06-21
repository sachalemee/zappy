/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** eggs
*/

#include "zappy.h"
#include <stdlib.h>

egg_t *spawn_egg(server_t *s, client_t *c)
{
    egg_t *egg = malloc(sizeof(egg_t));
    egg_t **tmp;

    if (!egg)
        return NULL;
    egg->id = s->next_egg_id++;
    egg->player_id = c->player->id;
    egg->x = c->player->x;
    egg->y = c->player->y;
    egg->team = c->player->team;
    egg->hatched = 0;
    egg->hatch_time = get_current_time() + get_action_duration(s->freq, 600);
    tmp = realloc(s->eggs, sizeof(egg_t *) * (s->nb_eggs + 1));
    if (!tmp) {
        free(egg);
        return NULL;
    }
    s->eggs = tmp;
    s->eggs[s->nb_eggs++] = egg;
    return egg;
}

void handle_eggs(server_t *s)
{
    long long current = get_current_time();
    egg_t *egg;

    for (int i = 0; i < s->nb_eggs; i++) {
        egg = s->eggs[i];
        if (!egg || egg->hatched)
            continue;
        if (current >= egg->hatch_time) {
            egg->hatched = 1;
            gui_event_eht(s, egg->id);
        }
    }
}
