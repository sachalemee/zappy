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
    server->map.tiles = NULL;
    server->players = NULL;
    server->clients = NULL;
    server->nb_clients = 0;
    server->eggs = NULL;
    server->nb_eggs = 0;
    server->next_player_id = 1;
    server->next_refill_time = get_current_time() + get_action_duration(server->freq, 20);
    server->next_egg_id = 0;
    if (init_map(server) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    return IS_EXIT_SUCCESS;
}
