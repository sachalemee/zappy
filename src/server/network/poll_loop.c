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
#include "zappy.h"

static void remove_client(server_t *s, int i)
{
    if (!s || !s->clients || !s->clients[i])
        return;
    close(s->clients[i]->fd);
    free(s->clients[i]);
    for (int j = i; j < s->nb_clients - 1; j++)
        s->clients[j] = s->clients[j + 1];
    s->nb_clients--;
    s->clients[s->nb_clients] = NULL;
}

static void handle_team_name(client_t *c, char *line)
{
    team_t *team;
    player_t *player;

    if (!c || !line)
        return;
    line[strcspn(line, "\r\n")] = '\0';
    if (strcmp(line, "GRAPHIC") == 0) {
        c->type = CLIENT_GUI;
        c->state = STATE_IN_GAME;
        dprintf(c->fd, "GRAPHIC_OK\n");
        return;
    }
    team = find_team(c->server, line);
    if (!team || team->free_slots <= 0) {
        write(c->fd, "ko\n", 3);
        return;
    }
    player = create_player(c->server, team);
    if (!player) {
        write(c->fd, "ko\n", 3);
        return;
    }
    team->free_slots--;
    if (add_player(c->server, player) == IS_EXIT_FAILURE) {
        free(player);
        write(c->fd, "ko\n", 3);
        return;
    }
    c->team = team;
    c->player = player;
    dprintf(c->fd, "%d\n", team->free_slots);
    dprintf(c->fd, "%d %d\n",
        c->server->width,
        c->server->height
    );
    c->state = STATE_IN_GAME;
}

static char *extract_line(client_t *c)
{
    static char line[2048];
    int i = 0;

    if (!c)
        return NULL;
    while (c->buffer[i] && c->buffer[i] != '\n')
        i++;
    if (!c->buffer[i])
        return NULL;
    strncpy(line, c->buffer, i);
    line[i] = '\0';
    memmove(c->buffer,
        c->buffer + i + 1,
        strlen(c->buffer + i + 1) + 1);
    c->buf_len -= (i + 1);
    if (c->buf_len < 0)
        c->buf_len = 0;
    return line;
}

static void handle_command(client_t *c, char *line)
{
    int delay;
    if (!c || !line)
        return;
    delay = get_command_delay(line, c->server->freq);
    if (delay >= 0)
        push_command(c, line, delay);
    else
        write(c->fd, "ko\n", 3);
}

static void handle_client(client_t *c)
{
    char buffer[1024];
    int len;
    char *line;

    if (!c)
        return;
    len = read(c->fd, buffer, sizeof(buffer) - 1);
    if (len <= 0) {
        c->fd = -1;
        return;
    }
    buffer[len] = '\0';
    if (c->buf_len + len < (int)sizeof(c->buffer) - 1) {
        strcat(c->buffer, buffer);
        c->buf_len += len;
    }
    while ((line = extract_line(c)) != NULL) {
        if (c->state == STATE_WAIT_TEAM)
            handle_team_name(c, line);
        else if (c->state == STATE_IN_GAME)
            handle_command(c, line);
    }
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
    for (int i = 0; i < s->nb_clients; i++) {
        if (s->clients[i])
            process_commands(s->clients[i]);
    }
}

int run_server(server_t *s)
{
    struct pollfd fds[1024];
    int nfds;

    printf("Server running...\n");
    while (1) {
        nfds = build_poll_fds(s, fds);
        if (poll(fds, nfds, -1) < 0)
            return IS_EXIT_FAILURE;

        process_poll_events(s, fds, nfds);
    }
    return IS_EXIT_SUCCESS;
}
