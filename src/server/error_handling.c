/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** error_handling
*/

#include "zappy.h"

int error_handling(int ac, char **av)
{
    (void)av;
    if (ac != 2)
        return helper();
    return IS_EXIT_SUCCESS;
}