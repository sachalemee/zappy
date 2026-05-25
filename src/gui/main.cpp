/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include "zappy.hpp"

int main(int ac, char **av)
{
    if (error_handling(ac, av) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    return IS_EXIT_SUCCESS;
}
