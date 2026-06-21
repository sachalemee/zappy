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
    egg->death_time = 0;
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
            egg->death_time = current + get_action_duration(s->freq, 1260);
            egg->team->free_slots++;
            gui_event_eht(s, egg->id);
        }
    }
}

void consume_egg(server_t *s, team_t *t, player_t *p)
{
    egg_t *e;

    for (int i = 0; i < s->nb_eggs; i++) {
        e = s->eggs[i];
        if (e && e->team == t && e->hatched) {
            p->x = e->x;
            p->y = e->y;
            gui_event_ebo(s, e->id);
            free(e);
            for (int j = i; j < s->nb_eggs - 1; j++)
                s->eggs[j] = s->eggs[j + 1];
            s->eggs[s->nb_eggs - 1] = NULL;
            s->nb_eggs--;
            return;
        }
    }
}

static void add_initial_egg(server_t *s, int team_idx)
{
    egg_t *e = malloc(sizeof(egg_t));
    egg_t **tmp;

    if (!e) return;
    e->id = s->next_egg_id++;
    e->player_id = -1;
    e->x = rand() % s->width;
    e->y = rand() % s->height;
    e->team = &s->teams[team_idx];
    e->hatched = 1;
    e->hatch_time = 0;
    e->death_time = get_current_time() + get_action_duration(s->freq, 1260);
    tmp = realloc(s->eggs, sizeof(egg_t *) * (s->nb_eggs + 1));
    if (!tmp) {
        free(e);
        return;
    }
    s->eggs = tmp;
    s->eggs[s->nb_eggs++] = e;
}

void generate_initial_eggs(server_t *s)
{
    for (int i = 0; i < s->nb_teams; i++) {
        for (int j = 0; j < s->clients_nb; j++) {
            add_initial_egg(s, i);
        }
    }
}

void destroy_egg(server_t *s, int egg_id)
{
    egg_t *e;

    for (int i = 0; i < s->nb_eggs; i++) {
        e = s->eggs[i];
        if (e && e->id == egg_id) {
            gui_event_edi(s, e->id);
            if (e->team && e->team->free_slots > 0)
                e->team->free_slots--;
            free(e);
            for (int j = i; j < s->nb_eggs - 1; j++)
                s->eggs[j] = s->eggs[j + 1];
            s->eggs[s->nb_eggs - 1] = NULL;
            s->nb_eggs--;
            return;
        }
    }
}
