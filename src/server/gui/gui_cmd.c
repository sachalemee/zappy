/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gui_cmd
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "zappy.h"

static void handle_time_cmd(server_t *s, client_t *c, char *cmd)
{
    char buf[64];
    int new_freq = -1;

    if (strncmp(cmd, "sst", 3) == 0) {
        if (sscanf(cmd, "sst %d", &new_freq) == 1 && new_freq > 0)
            s->freq = new_freq;
    }
    snprintf(buf, sizeof(buf), "sgt %d\n", s->freq);
    write(c->fd, buf, strlen(buf));
}

static int handle_map_cmd(server_t *s, client_t *c, char *cmd)
{
    int x = -1;
    int y = -1;

    if (strncmp(cmd, "mct", 3) == 0) {
        gui_send_mct(s);
        return 1;
    }
    if (strncmp(cmd, "bct", 3) == 0) {
        if (sscanf(cmd, "bct %d %d", &x, &y) != 2) return 1;
        if (x < 0 || x >= s->width || y < 0 || y >= s->height) return 1;
        gui_send_bct(s, x, y);
        return 1;
    }
    return 0;
}

void handle_gui_command(server_t *s, client_t *c)
{
    char *cmd = c->cmds[0];

    if (handle_map_cmd(s, c, cmd))
        return;
    if (strncmp(cmd, "sgt", 3) == 0 || strncmp(cmd, "sst", 3) == 0) {
        handle_time_cmd(s, c, cmd);
        return;
    }
    write(c->fd, "suc\n", 4);
}
