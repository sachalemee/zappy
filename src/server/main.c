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
    if (error_handling(ac, av, &server) == IS_EXIT_FAILURE) {
        write(2, "[ERROR] Invalid arguments\n", 27);
        return IS_EXIT_FAILURE;
    }
    if (init_server(&server) == IS_EXIT_FAILURE) {
        write(2, "[ERROR] Server init failed\n", 27);
        return IS_EXIT_FAILURE;
    }
    if (create_socket(&server) == IS_EXIT_FAILURE) {
        write(2, "[ERROR] Socket creation failed\n", 31);
        return IS_EXIT_FAILURE;
    }
    if (bind_socket(&server) == IS_EXIT_FAILURE) {
        write(2, "[ERROR] Bind failed\n", 21);
        return IS_EXIT_FAILURE;
    }
    if (listen_socket(&server) == IS_EXIT_FAILURE) {
        write(2, "[ERROR] Listen failed\n", 23);
        return IS_EXIT_FAILURE;
    }
    if (run_server(&server) == IS_EXIT_FAILURE) {
        write(2, "[ERROR] Server runtime error\n", 30);
        return IS_EXIT_FAILURE;
    }
    destroy_server(&server);
    return IS_EXIT_SUCCESS;
}
