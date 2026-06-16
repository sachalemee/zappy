/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** resource_utils
*/

#include <string.h>
#include "zappy.h"

int get_resource_type(const char *str)
{
    if (strcmp(str, "food") == 0)
        return FOOD;
    if (strcmp(str, "linemate") == 0)
        return LINEMATE;
    if (strcmp(str, "deraumere") == 0)
        return DERAUMERE;
    if (strcmp(str, "sibur") == 0)
        return SIBUR;
    if (strcmp(str, "mendiane") == 0)
        return MENDIANE;
    if (strcmp(str, "phiras") == 0)
        return PHIRAS;
    if (strcmp(str, "thystame") == 0)
        return THYSTAME;
    return -1;
}
