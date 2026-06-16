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
    {"Take", 7, cmd_take},
    {"Set", 7, cmd_set},
    {"Broadcast", 7, cmd_broadcast},
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
    client_t *c;
    char *cmd;

    for (int i = 0; i < s->nb_clients; i++) {
        c = s->clients[i];
        if (!c)
            continue;
        if (c->type == CLIENT_GUI) {
            if (c->nb_cmds <= 0)
                continue;
            cmd = c->cmds[0];
            if (strncmp(cmd, "mct", 3) == 0 ||
                strncmp(cmd, "bct", 3) == 0 ||
                strncmp(cmd, "sgt", 3) == 0) {
                handle_gui_command(s, c);
            } else if (strncmp(cmd, "pin", 3) == 0 ||
                     strncmp(cmd, "plv", 3) == 0 ||
                     strncmp(cmd, "ppo", 3) == 0) {
                handle_gui_player_command(s, c);
            } else {
                write(c->fd, "suc\n", 4);
            }
            pop_command(c);
            continue;
        }
        if (c->state != STATE_IN_GAME)
            continue;
        start_command(c);
        finish_command(c);
    }
}
