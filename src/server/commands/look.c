/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** look
*/

#include "commands.h"
#include <unistd.h>
#include <string.h>

static void get_target(player_t *p, int d, int w, int t[2])
{
    if (p->orientation == 1) {
        t[0] = p->x + w;
        t[1] = p->y - d;
    } else if (p->orientation == 2) {
        t[0] = p->x + d;
        t[1] = p->y + w;
    } else if (p->orientation == 3) {
        t[0] = p->x - w;
        t[1] = p->y + d;
    } else {
        t[0] = p->x - d;
        t[1] = p->y - w;
    }
}

static void append_str(char *buf, const char *str, int *len)
{
    int slen = strlen(str);

    if (*len + slen < 8191) {
        strcat(buf, str);
        *len += slen;
    }
}

static void append_p(server_t *s, int t[2], char *buf, int *len)
{
    for (int i = 0; i < s->nb_players; i++) {
        if (s->players[i] && s->players[i]->x == t[0] &&
            s->players[i]->y == t[1]) {
            append_str(buf, "player ", len);
        }
    }
}

static void append_r(server_t *s, int t[2], char *buf, int *len)
{
    tile_t *tile = &s->map.tiles[t[1]][t[0]];
    const char *n[] = {"food ", "linemate ", "deraumere ",
        "sibur ", "mendiane ", "phiras ", "thystame "};

    for (int i = 0; i < RESOURCE_MAX; i++) {
        for (int k = 0; k < tile->resources[i]; k++) {
            append_str(buf, n[i], len);
        }
    }
    if (*len > 0 && buf[*len - 1] == ' ') {
        buf[*len - 1] = '\0';
        (*len)--;
    }
}

void cmd_look(server_t *s, client_t *c)
{
    char buf[8192] = {0};
    int len = 0;
    int t[2];

    if (!c->player)
        return;
    append_str(buf, "[", &len);
    for (int d = 0; d <= c->player->level; d++) {
        for (int w = -d; w <= d; w++) {
            get_target(c->player, d, w, t);
            t[0] = (t[0] % s->width + s->width) % s->width;
            t[1] = (t[1] % s->height + s->height) % s->height;
            append_p(s, t, buf, &len);
            append_r(s, t, buf, &len);
            if (!(d == c->player->level && w == d))
                append_str(buf, ",", &len);
        }
    }
    append_str(buf, "]\n", &len);
    write(c->fd, buf, len);
}
