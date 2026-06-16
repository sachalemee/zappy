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

void handle_gui_command(server_t *s, client_t *c)
{
    char *cmd = c->cmds[0];
    int x;
    int y;
    char buf[64];

    if (strncmp(cmd, "mct", 3) == 0) {
        gui_send_mct(s);
        return;
    }
    if (strncmp(cmd, "bct", 3) == 0) {
        x = -1;
        y = -1;
        if (sscanf(cmd, "bct %d %d", &x, &y) != 2)
            return;
        if (x < 0 || x >= s->width || y < 0 || y >= s->height)
            return;
        gui_send_bct(s, x, y);
        return;
    }
    if (strncmp(cmd, "sgt", 3) == 0) {
        snprintf(buf, sizeof(buf), "sgt %d\n", s->freq);
        write(c->fd, buf, strlen(buf));
        return;
    }
    write(c->fd, "suc\n", 4);
}
