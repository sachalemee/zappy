/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** helper
*/

#include "zappy.h"
#include <stdio.h>

int helper(void)
{
    dprintf(2, "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n");
    return IS_EXIT_FAILURE;
}
