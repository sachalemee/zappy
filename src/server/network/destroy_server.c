/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** destroy_server
*/

#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"

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
    for (int i = 0; teams[i].name != NULL; i++) {
        free(teams[i].name);
    }
    free(teams);
}

static void free_players(server_t *s)
{
    if (!s || !s->players)
        return;
    for (int i = 0; i < s->nb_players; i++) {
        if (s->players[i])
            free(s->players[i]);
    }
    free(s->players);
}

static void free_clients(server_t *s)
{
    if (!s || !s->clients)
        return;
    for (int i = 0; i < s->nb_clients; i++) {
        if (s->clients[i]) {
            if (s->clients[i]->fd != -1)
                close(s->clients[i]->fd);
            free(s->clients[i]);
        }
    }
    free(s->clients);
}

static void free_eggs(server_t *s)
{
    if (!s || !s->eggs)
        return;
    for (int i = 0; i < s->nb_eggs; i++) {
        if (s->eggs[i])
            free(s->eggs[i]);
    }
    free(s->eggs);
}

void destroy_server(server_t *server)
{
    if (!server)
        return;
    if (server->server_fd != -1)
        close(server->server_fd);
    free_map(&server->map);
    free_teams(server->teams);
    free_players(server);
    free_clients(server);
    free_eggs(server);
}