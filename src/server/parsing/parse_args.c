/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** parse_args
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "zappy.h"

typedef struct arg_map_s {
    char *flag;
    size_t offset;
} arg_map_t;

static const arg_map_t INT_ARGS[] = {
    {"-p", offsetof(server_t, port)},
    {"-x", offsetof(server_t, width)},
    {"-y", offsetof(server_t, height)},
    {"-c", offsetof(server_t, clients_nb)},
    {"-f", offsetof(server_t, freq)},
    {NULL, 0}
};

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

static team_t *build_teams(char **names)
{
    int count = count_teams(names);
    team_t *teams = malloc(sizeof(team_t) * (count + 1));

    if (!teams)
        return NULL;
    for (int i = 0; i < count; i++) {
        teams[i].name = strdup(names[i]);
        teams[i].free_slots = 0;
        if (!teams[i].name)
            return NULL;
    }
    teams[count].name = NULL;
    return teams;
}

static int handle_teams(char **av, int *i, server_t *s)
{
    char **names;

    (*i)++;
    names = parse_teams(av, i);
    if (!names)
        return IS_EXIT_FAILURE;
    
    if (s->clients_nb <= 0)
        s->clients_nb = 1;
        
    s->teams = build_teams(names);
    if (!s->teams) {
        free(names);
        return IS_EXIT_FAILURE;
    }
    s->nb_teams = count_teams(names);
    free(names);
    (*i)--;
    return IS_EXIT_SUCCESS;
}

static int parse_single_arg(char **av, int *i, server_t *s)
{
    int *target;

    if (strcmp(av[*i], "-n") == 0)
        return handle_teams(av, i, s);
    for (int j = 0; INT_ARGS[j].flag; j++) {
        if (strcmp(av[*i], INT_ARGS[j].flag) == 0) {
            target = (int *)((char *)s + INT_ARGS[j].offset);
            *target = get_int(av, i);
            return IS_EXIT_SUCCESS;
        }
    }
    return IS_EXIT_FAILURE;
}

int parse_args(int ac, char **av, server_t *s)
{
    for (int i = 1; i < ac; i++) {
        if (parse_single_arg(av, &i, s) == IS_EXIT_FAILURE)
            return IS_EXIT_FAILURE;
    }
    for (int i = 0; i < s->nb_teams; i++) {
        s->teams[i].free_slots = s->clients_nb;
    }
    return IS_EXIT_SUCCESS;
}
