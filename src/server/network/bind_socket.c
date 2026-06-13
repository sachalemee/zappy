/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** bind_socket
*/

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "zappy.h"

int bind_socket(server_t *s)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(s->port);
    if (bind(s->server_fd,
        (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return IS_EXIT_FAILURE;
    return IS_EXIT_SUCCESS;
}
