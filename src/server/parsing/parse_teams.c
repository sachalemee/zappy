/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** parse_teams
*/

#include <stdlib.h>
#include "zappy.h"

static int is_flag(char *str)
{
    return (str && str[0] == '-');
}

static int count_teams(char **av, int i)
{
    int count = 0;

    while (av[i] && !is_flag(av[i])) {
        count++;
        i++;
    }
    return count;
}

char **parse_teams(char **av, int *i)
{
    int count = count_teams(av, *i);
    char **teams = malloc(sizeof(char *) * (count + 1));

    if (!teams || count <= 0)
        return NULL;
    for (int j = 0; j < count; j++) {
        if (!av[*i] || is_flag(av[*i]))
            return NULL;
        teams[j] = av[*i];
        (*i)++;
    }
    teams[count] = NULL;
    return teams;
}
