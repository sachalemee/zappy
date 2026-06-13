/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** execute_actions
*/

#include <unistd.h>
#include "zappy.h"

static void start_command(client_t *c)
{
    long long duration;

    if (c->nb_cmds == 0 || c->action_end_time > 0)
        return;
    
    duration = get_action_duration(c->server->freq, 7);
    c->action_end_time = get_current_time() + duration;
}

static void finish_command(client_t *c)
{
    long long current = get_current_time();

    if (c->action_end_time == 0 || current < c->action_end_time)
        return;
    
    write(c->fd, "ok\n", 3);
    c->action_end_time = 0;
    pop_command(c);
}

void handle_active_commands(server_t *s)
{
    for (int i = 0; i < s->nb_clients; i++) {
        if (!s->clients[i] || s->clients[i]->state != STATE_IN_GAME)
            continue;
        start_command(s->clients[i]);
        finish_command(s->clients[i]);
    }
}
