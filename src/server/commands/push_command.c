/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** push_command
*/

#include <stdlib.h>
#include <string.h>
#include "zappy.h"

static command_t *create_cmd(char *name, int delay)
{
    command_t *cmd = malloc(sizeof(command_t));

    if (!cmd)
        return NULL;

    cmd->name = strdup(name);
    cmd->delay = delay;
    cmd->next = NULL;

    return cmd;
}

void push_command(client_t *c, char *name, int delay)
{
    command_t *new_cmd = create_cmd(name, delay);
    command_t *tmp;

    if (!new_cmd)
        return;
    if (!c->cmd_queue) {
        c->cmd_queue = new_cmd;
        return;
    }
    tmp = c->cmd_queue;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_cmd;
}
