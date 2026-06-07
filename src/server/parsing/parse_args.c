/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** parse_args
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zappy.h"

static int is_flag(char *str)
{
    return (str && str[0] == '-');
}

static int is_number(char *str)
{
    if (!str)
        return 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}

static int get_int(char **av, int *i)
{
    if (!av[*i + 1] || is_flag(av[*i + 1]) || !is_number(av[*i + 1]))
        return -1;
    return atoi(av[++(*i)]);
}

static int count_teams(char **names)
{
    int i = 0;

    while (names && names[i])
        i++;
    return i;
}

static team_t *build_teams(char **names, int clients_nb)
{
    int count = count_teams(names);
    team_t *teams = malloc(sizeof(team_t) * (count + 1));

    if (!teams)
        return NULL;
    for (int i = 0; i < count; i++) {
        teams[i].name = strdup(names[i]);
        teams[i].free_slots = clients_nb;
        if (!teams[i].name)
            return NULL;
    }
    teams[count].name = NULL;
    return teams;
}

int parse_args(int ac, char **av, server_t *s)
{
    for (int i = 1; i < ac; i++) {
        if (strcmp(av[i], "-p") == 0)
            s->port = get_int(av, &i);
        else if (strcmp(av[i], "-x") == 0)
            s->width = get_int(av, &i);
        else if (strcmp(av[i], "-y") == 0)
            s->height = get_int(av, &i);
        else if (strcmp(av[i], "-c") == 0)
            s->clients_nb = get_int(av, &i);
        else if (strcmp(av[i], "-f") == 0)
            s->freq = get_int(av, &i);
        else if (strcmp(av[i], "-n") == 0) {
            i++;
            char **names = parse_teams(av, &i);

            if (!names)
                return IS_EXIT_FAILURE;
            s->teams = build_teams(names, s->clients_nb);
            if (!s->teams)
                return IS_EXIT_FAILURE;
            s->nb_teams = count_teams(names);
            i--;
        }
        else
            return IS_EXIT_FAILURE;
    }
    return IS_EXIT_SUCCESS;
}
