/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** add_player
*/

#include <stdlib.h>
#include "zappy.h"

int add_player(server_t *s, player_t *p)
{
    player_t **tmp;

    tmp = realloc(s->players,
        sizeof(player_t *) * (s->nb_players + 1));
    if (!tmp)
        return IS_EXIT_FAILURE;
    s->players = tmp;
    s->players[s->nb_players] = p;
    s->nb_players++;
    return IS_EXIT_SUCCESS;
}
