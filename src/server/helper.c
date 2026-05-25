/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** helper
*/

#include <stdio.h>
#include <unistd.h>
#include "zappy.h"

/*
    option                  description

    -p port                 port number
    -x width                width of the world
    -y height               height of the world
    -n name1 name2 ...      name of the team
    -c clientsNb            number of authorized clients per team
    -f freq                 reciprocal of time unit for execution of actions
*/
int helper()
{
    dprintf(STDERR_FILENO, "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n");
    return IS_EXIT_FAILURE;
}
