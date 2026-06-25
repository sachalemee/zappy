/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** listen_socket
*/

#include <sys/socket.h>
#include "zappy.h"

int listen_socket(server_t *s)
{
    if (listen(s->server_fd, 100) < 0)
        return IS_EXIT_FAILURE;
    return IS_EXIT_SUCCESS;
}
