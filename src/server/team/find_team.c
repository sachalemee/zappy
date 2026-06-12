/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** find_team
*/

#include <string.h>
#include "zappy.h"

team_t *find_team(server_t *s, const char *name)
{
    for (int i = 0; i < s->nb_teams; i++) {
        if (strcmp(s->teams[i].name, name) == 0)
            return &s->teams[i];
    }
    return NULL;
}