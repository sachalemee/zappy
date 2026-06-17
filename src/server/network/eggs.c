/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** eggs
*/

#include "zappy.h"

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
            egg->team->free_slots++;
            gui_event_eht(s, egg->id);
        }
    }
}