/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** gui_broadcast
*/

#include <string.h>
#include <unistd.h>
#include "zappy.h"

void send_to_gui(server_t *s, const char *msg)
{
    int len = strlen(msg);

    for (int i = 0; i < s->nb_clients; i++) {
        if (s->clients[i] && 
            s->clients[i]->type == CLIENT_GUI && 
            s->clients[i]->state == STATE_IN_GAME) {
            write(s->clients[i]->fd, msg, len);
        }
    }
}
