/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** execute_command
*/

#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "zappy.h"

static const cmd_map_t CMD_TABLE[] = {
    {"Forward", 7, cmd_forward},
    {"Right", 7, cmd_right},
    {"Left", 7, cmd_left},
    {"Look", 7, cmd_look},
    {"Inventory", 1, cmd_inventory},
    {NULL, 0, NULL}
};

static const cmd_map_t *get_cmd(char *cmd_str)
{
    for (int i = 0; CMD_TABLE[i].name; i++) {
        if (strncmp(cmd_str, CMD_TABLE[i].name, 
            strlen(CMD_TABLE[i].name)) == 0)
            return &CMD_TABLE[i];
    }
    return NULL;
}

static void start_command(client_t *c)
{
    const cmd_map_t *cmd;

    if (c->nb_cmds == 0 || c->action_end_time > 0)
        return;
    cmd = get_cmd(c->cmds[0]);
    if (!cmd) {
        write(c->fd, "ko\n", 3);
        pop_command(c);
        return;
    }
    c->action_end_time = get_current_time() + 
        get_action_duration(c->server->freq, cmd->time_cost);
}

static void finish_command(client_t *c)
{
    const cmd_map_t *cmd;
    long long current = get_current_time();

    if (c->nb_cmds == 0 || c->action_end_time == 0 || 
        current < c->action_end_time)
        return;
    cmd = get_cmd(c->cmds[0]);
    if (cmd && cmd->func)
        cmd->func(c->server, c);
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
