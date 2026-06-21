/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** fork
*/

#include <stdlib.h>
#include <unistd.h>
#include "commands.h"

int pre_cmd_fork(server_t *s, client_t *c)
{
    (void)s;
    if (!c->player)
        return -1;
    gui_event_pfk(s, c->player);
    return 0;
}

void cmd_fork(server_t *s, client_t *c)
{
    egg_t *egg;
    egg_t **tmp;

    if (!c->player || !c->player->team)
        return;
    egg = malloc(sizeof(egg_t));
    if (!egg)
        return;
    egg->id = s->next_egg_id++;
    egg->player_id = c->player->id;
    egg->x = c->player->x;
    egg->y = c->player->y;
    egg->team = c->player->team;
    egg->hatched = 0;
    egg->hatch_time = get_current_time()
        + get_action_duration(s->freq, 600);
    tmp = realloc(s->eggs, sizeof(egg_t *) * (s->nb_eggs + 1));
    if (!tmp) {
        free(egg);
        return;
    }
    s->eggs = tmp;
    s->eggs[s->nb_eggs++] = egg;
    c->player->team->free_slots++;
    gui_event_enw(s, egg->id, c->player);
    write(c->fd, "ok\n", 3);
}
