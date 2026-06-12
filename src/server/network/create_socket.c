/*
** EPITECH PROJECT, 2026
** *
** File description:
** create_socket
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "zappy.h"

int create_socket(server_t *s)
{
    int opt = 1;

    s->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s->server_fd == -1)
        return IS_EXIT_FAILURE;
    if (setsockopt(s->server_fd, SOL_SOCKET,
        SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        return IS_EXIT_FAILURE;
    return IS_EXIT_SUCCESS;
}
