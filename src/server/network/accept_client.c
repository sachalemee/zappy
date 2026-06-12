/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** accept_client
*/

#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "zappy.h"

static client_t *create_client(int fd, server_t *s)
{
    client_t *c = malloc(sizeof(client_t));

    if (!c)
        return NULL;
    c->fd = fd;
    c->state = STATE_WAIT_TEAM;
    c->type = CLIENT_AI;    
    c->buf_len = 0;
    c->server = s;
    c->team = NULL;
    c->player = NULL;
    c->buffer[0] = '\0';
    return c;
}

static int add_client(server_t *s, client_t *c)
{
    client_t **tmp;

    tmp = realloc(s->clients,
        sizeof(client_t *) * (s->nb_clients + 1));
    if (!tmp)
        return IS_EXIT_FAILURE;
    s->clients = tmp;
    s->clients[s->nb_clients] = c;
    s->nb_clients++;
    return IS_EXIT_SUCCESS;
}

int accept_client(server_t *s)
{
    int client_fd = accept(s->server_fd, NULL, NULL);
    client_t *c;

    if (client_fd < 0)
        return IS_EXIT_FAILURE;
    c = create_client(client_fd, s);
    if (!c) {
        close(client_fd);
        return IS_EXIT_FAILURE;
    }
    if (add_client(s, c) == IS_EXIT_FAILURE) {
        free(c);
        close(client_fd);
        return IS_EXIT_FAILURE;
    }
    write(client_fd, "WELCOME\n", 8);
    return IS_EXIT_SUCCESS;
}
