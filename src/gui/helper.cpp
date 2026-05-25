/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** helper
*/

#include "zappy.hpp"
#include <iostream>

/*
    option                  description

    -p port                 port number
    -h machine              hostname of the server
*/
int helper()
{
    std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
    return IS_EXIT_FAILURE;
}
