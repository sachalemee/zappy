/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** destroy_server
*/

#include <stdlib.h>
#include "zappy.h"
#include <unistd.h>

static void free_map(map_t *map)
{
    if (!map || !map->tiles)
        return;
    for (int i = 0; i < map->height; i++) {
        if (map->tiles[i])
            free(map->tiles[i]);
    }
    free(map->tiles);
}

static void free_teams(team_t *teams)
{
    if (!teams)
        return;
    free(teams);
}

static void free_players(player_t **players)
{
    if (!players)
        return;
    free(players);
}

void destroy_server(server_t *server)
{
    if (!server)
        return;
    if (server->server_fd != -1)
        close(server->server_fd);
    free_map(&server->map);
    free_teams(server->teams);
    free_players(server->players);
}