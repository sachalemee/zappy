/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** create_player
*/

#include <stdlib.h>
#include <string.h>
#include "zappy.h"

player_t *create_player(server_t *s, team_t *team)
{
    player_t *p;

    if (!s || !team || s->width <= 0 || s->height <= 0)
        return NULL;
    p = malloc(sizeof(player_t));
    if (!p)
        return NULL;
    memset(p, 0, sizeof(player_t));
    p->id = s->next_player_id++;
    p->x = rand() % s->width;
    p->y = rand() % s->height;
    p->orientation = (rand() % 4) + 1;
    p->level = 1;
    p->team = team;
    p->inventory[FOOD] = 10;
    return p;
}
