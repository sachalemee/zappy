/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** helper
*/

#include <stdio.h>
#include <unistd.h>
#include "zappy.h"

int helper()
{
    dprintf(STDERR_FILENO, "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n");
    return IS_EXIT_FAILURE;
}
