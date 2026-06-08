/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <stdlib.h>
#include <time.h>
#include "zappy.h"

int main(int ac, char **av)
{
    server_t server = {0};

    srand(time(NULL));
    if (error_handling(ac, av, &server) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    if (init_server(&server) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    if (create_socket(&server) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    if (bind_socket(&server) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    if (listen_socket(&server) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    if (run_server(&server) == IS_EXIT_FAILURE)
        return IS_EXIT_FAILURE;
    destroy_server(&server);
    return IS_EXIT_SUCCESS;
}