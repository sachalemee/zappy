/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** execute_command
*/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "commands.h"
#include "zappy.h"

static void execute(client_t *c, command_t *cmd)
{
    if (strcmp(cmd->name, "Forward") == 0)
        cmd_forward(c->server, c);
    else if (strcmp(cmd->name, "Right") == 0)
        cmd_right(c);
    else if (strcmp(cmd->name, "Left") == 0)
        cmd_left(c);
    else if (strcmp(cmd->name, "Look") == 0)
        cmd_look(c);
    else if (strcmp(cmd->name, "Inventory") == 0)
        cmd_inventory(c);
}

void process_commands(client_t *c)
{
    command_t *cmd;

    if (!c->cmd_queue)
        return;
    cmd = c->cmd_queue;
    if (--cmd->delay > 0)
        return;
    execute(c, cmd);
    c->cmd_queue = cmd->next;
    free(cmd->name);
    free(cmd);
}
