/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** init_server
*/

#include <stdlib.h>
#include "zappy.h"

int init_server(server_t *server)
{
    if (!server)
        return IS_EXIT_FAILURE;
    server->server_fd = -1;
    server->nb_players = 0;
    server->map.width = 0;
    server->map.height = 0;
    server->map.tiles = NULL;
    server->teams = NULL;
    server->nb_teams = 0;
    server->players = NULL;
    return IS_EXIT_SUCCESS;
}
