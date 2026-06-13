/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** init_map
*/

#include <stdlib.h>
#include "zappy.h"

static void init_tile(tile_t *tile)
{
    for (int k = 0; k < RESOURCE_MAX; k++)
        tile->resources[k] = 0;
}

static int alloc_tiles(map_t *map)
{
    map->tiles = malloc(sizeof(tile_t *) * map->height);
    if (!map->tiles)
        return IS_EXIT_FAILURE;
    for (int i = 0; i < map->height; i++) {
        map->tiles[i] = malloc(sizeof(tile_t) * map->width);
        if (!map->tiles[i])
            return IS_EXIT_FAILURE;
        for (int j = 0; j < map->width; j++)
            init_tile(&map->tiles[i][j]);
    }
    return IS_EXIT_SUCCESS;
}

int init_map(server_t *s)
{
    if (s->width <= 0 || s->height <= 0)
        return IS_EXIT_FAILURE;
    s->map.width = s->width;
    s->map.height = s->height;
    if (alloc_tiles(&s->map) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    refill_map(s);
    return IS_EXIT_SUCCESS;
}
