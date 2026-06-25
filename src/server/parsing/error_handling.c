/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** error_handling
*/

#include "zappy.h"

int error_handling(int ac, char **av, server_t *s)
{
    if (ac < 2) {
        write(2, "[ERROR] Not enough arguments\n", 30);
        return helper();
    }
    if (parse_args(ac, av, s) == IS_EXIT_FAILURE) {
        write(2, "[ERROR] Failed to parse arguments\n", 34);
        return helper();
    }
    if (s->port <= 0) {
        write(2, "[ERROR] Invalid port\n", 22);
        return IS_EXIT_FAILURE;
    }
    if (s->width <= 0) {
        write(2, "[ERROR] Invalid width\n", 23);
        return IS_EXIT_FAILURE;
    }
    if (s->height <= 0) {
        write(2, "[ERROR] Invalid height\n", 24);
        return IS_EXIT_FAILURE;
    }
    if (s->clients_nb <= 0) {
        write(2, "[ERROR] Invalid clients number\n", 32);
        return IS_EXIT_FAILURE;
    }
    if (!s->teams || s->nb_teams <= 0) {
        write(2, "[ERROR] No teams defined\n", 26);
        return IS_EXIT_FAILURE;
    }
    return IS_EXIT_SUCCESS;
}
