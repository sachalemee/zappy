/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** poll_loop
*/

#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "zappy.h"

static volatile int server_is_running = 1;

static void sigint_handler(int dummy)
{
    (void)dummy;
    server_is_running = 0;
}

static void remove_client(server_t *s, int i)
{
    if (!s->clients || !s->clients[i])
        return;
    if (s->clients[i]->fd != -1)
        close(s->clients[i]->fd);
    free(s->clients[i]);
    for (int j = i; j < s->nb_clients - 1; j++) {
        s->clients[j] = s->clients[j + 1];
    }
    s->clients[s->nb_clients - 1] = NULL;
    s->nb_clients--;
}

static void handle_client(client_t *c)
{
    char tmp[1024];
    int len;

    len = read(c->fd, tmp, sizeof(tmp) - 1);
    if (len <= 0) {
        c->fd = -1;
        return;
    }
    tmp[len] = '\0';
    if (c->buf_len + len < 1023) {
        strncat(c->buffer, tmp, len);
        c->buf_len += len;
    }
    extract_commands(c);
    if (c->state == STATE_WELCOME)
        handle_handshake(c);
}

static int build_poll_fds(server_t *s, struct pollfd *fds)
{
    int idx = 0;

    fds[idx].fd = s->server_fd;
    fds[idx].events = POLLIN;
    idx++;
    for (int i = 0; i < s->nb_clients; i++) {
        if (s->clients[i] && s->clients[i]->fd > 0) {
            fds[idx].fd = s->clients[i]->fd;
            fds[idx].events = POLLIN;
            idx++;
        }
    }
    return idx;
}

static void process_client(server_t *s, int fd)
{
    for (int j = 0; j < s->nb_clients; j++) {
        if (s->clients[j] && s->clients[j]->fd == fd) {
            handle_client(s->clients[j]);
            if (s->clients[j]->fd == -1)
                remove_client(s, j);
            break;
        }
    }
}

static void process_poll_events(server_t *s, struct pollfd *fds, int nfds)
{
    if (fds[0].revents & POLLIN)
        accept_client(s);
    for (int i = 1; i < nfds; i++) {
        if (fds[i].revents & POLLIN)
            process_client(s, fds[i].fd);
    }
}

int run_server(server_t *s)
{
    struct pollfd fds[1024];
    int nfds;
    int timeout;

    signal(SIGINT, sigint_handler);
    printf("Server running...\n");
    while (server_is_running) {
        handle_active_commands(s);
        nfds = build_poll_fds(s, fds);
        timeout = calculate_timeout(s);
        if (poll(fds, nfds, timeout) < 0) {
            if (!server_is_running)
                break;
            return IS_EXIT_FAILURE;
        }
        process_poll_events(s, fds, nfds);
    }
    return IS_EXIT_SUCCESS;
}
