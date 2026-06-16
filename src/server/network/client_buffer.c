/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** client_buffer
*/

#include <string.h>
#include <stdio.h>
#include "zappy.h"

static void shift_buffer(client_t *c, int shift_len)
{
    int remaining = c->buf_len - shift_len;

    if (remaining > 0)
        memmove(c->buffer, c->buffer + shift_len, remaining);
    c->buf_len = remaining;
    c->buffer[remaining] = '\0';
}

static int store_raw_cmd(client_t *c, int cmd_len)
{
    if (c->nb_cmds >= MAX_CMDS)
        return 0;
    strncpy(c->cmds[c->nb_cmds], c->buffer, cmd_len);
    if (cmd_len > 0 && c->cmds[c->nb_cmds][cmd_len - 1] == '\r')
        c->cmds[c->nb_cmds][cmd_len - 1] = '\0';
    else
        c->cmds[c->nb_cmds][cmd_len] = '\0';
    c->nb_cmds++;
    return 1;
}

void extract_commands(client_t *c)
{
    int nl_pos = -1;

    while (c->nb_cmds < MAX_CMDS) {
        nl_pos = -1;
        for (int i = 0; i < c->buf_len; i++) {
            if (c->buffer[i] == '\n') {
                nl_pos = i;
                break;
            }
        }
        if (nl_pos == -1)
            break;
        if (c->type == CLIENT_GUI) {
            strncpy(c->cmds[c->nb_cmds], c->buffer, nl_pos);
            c->cmds[c->nb_cmds][nl_pos] = '\0';
            c->nb_cmds++;
        } else {
            store_raw_cmd(c, nl_pos);
        }
        shift_buffer(c, nl_pos + 1);
    }
}

void pop_command(client_t *c)
{
    if (c->nb_cmds <= 0)
        return;
    for (int i = 0; i < c->nb_cmds - 1; i++) {
        strncpy(c->cmds[i], c->cmds[i + 1], MAX_CMD_LEN);
    }
    c->nb_cmds--;
}
