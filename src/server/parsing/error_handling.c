/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** error_handling
*/

#include "zappy.h"

int error_handling(int ac, char **av, server_t *s)
{
    if (ac < 2)
        return helper();
    if (parse_args(ac, av, s) == IS_EXIT_FAILURE)
        return helper();
    if (s->port <= 0)
        return IS_EXIT_FAILURE;
    if (s->width <= 0)
        return IS_EXIT_FAILURE;
    if (s->height <= 0)
        return IS_EXIT_FAILURE;
    if (s->clients_nb <= 0)
        return IS_EXIT_FAILURE;
    if (s->freq <= 0)
        return IS_EXIT_FAILURE;
    if (!s->teams || s->nb_teams <= 0)
        return IS_EXIT_FAILURE;
    return IS_EXIT_SUCCESS;
}
