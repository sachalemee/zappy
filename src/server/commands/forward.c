/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** forward
*/

#include "commands.h"
#include <unistd.h>

void cmd_forward(server_t *s, client_t *c)
{
    player_t *p = c->player;

    if (!p)
        return;
    switch (p->orientation) {
        case 1:
            p->y = (p->y - 1 + s->height) % s->height;
            break;
        case 2:
            p->x = (p->x + 1) % s->width;
            break;
        case 3:
            p->y = (p->y + 1) % s->height;
            break;
        case 4:
            p->x = (p->x - 1 + s->width) % s->width;
            break;
    }
    gui_event_ppo(s, p);
    write(c->fd, "ok\n", 3);
}
