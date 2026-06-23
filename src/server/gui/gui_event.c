/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gui_event
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"

static void gui_send(server_t *s, const char *msg)
{
    send_to_gui(s, msg);
}

void gui_event_bct(server_t *s, int x, int y)
{
    gui_send_bct(s, x, y);
}

void gui_event_mct(server_t *s)
{
    gui_send_mct(s);
}

void gui_event_pnw(server_t *s, player_t *p)
{
    char buf[128];

    snprintf(buf, sizeof(buf),
        "pnw #%d %d %d %d %d %s\n",
        p->id, p->x, p->y,
        p->orientation, p->level,
        p->team->name);
    gui_send(s, buf);
}

void gui_event_ppo(server_t *s, player_t *p)
{
    gui_send_ppo(s, p);
}

void gui_event_pin(server_t *s, player_t *p)
{
    char buf[256];

    snprintf(buf, sizeof(buf),
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        p->id, p->x, p->y,
        p->inventory[FOOD], p->inventory[LINEMATE],
        p->inventory[DERAUMERE], p->inventory[SIBUR],
        p->inventory[MENDIANE], p->inventory[PHIRAS],
        p->inventory[THYSTAME]);
    send_to_gui(s, buf);
}

void gui_event_plv(server_t *s, player_t *p)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "plv #%d %d\n", p->id, p->level);
    gui_send(s, buf);
}

void gui_event_pgt(server_t *s, player_t *p, int res)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "pgt #%d %d\n", p->id, res);
    gui_send(s, buf);
}

void gui_event_pdr(server_t *s, player_t *p, int res)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "pdr #%d %d\n", p->id, res);
    gui_send(s, buf);
}

void gui_event_pbc(server_t *s, player_t *p, const char *msg)
{
    char buf[256];

    snprintf(buf, sizeof(buf), "pbc #%d %s\n", p->id, msg);
    gui_send(s, buf);
}

void gui_event_pic(server_t *s, player_t *p)
{
    char buf[256];
    int len;
    client_t *o;

    len = snprintf(buf, sizeof(buf), "pic %d %d %d", p->x, p->y, p->level);
    for (int i = 0; i < s->nb_clients; i++) {
        o = s->clients[i];
        if (o && o->type == CLIENT_AI && o->player &&
            o->player->x == p->x && o->player->y == p->y &&
            o->player->level == p->level) {
            len += snprintf(buf + len, sizeof(buf) - len, " #%d",
                o->player->id);
        }
    }
    snprintf(buf + len, sizeof(buf) - len, "\n");
    gui_send(s, buf);
}

void gui_event_pie(server_t *s, player_t *p, int result)
{
    char buf[128];

    snprintf(buf, sizeof(buf), "pie %d %d %d\n", p->x, p->y, result);
    gui_send(s, buf);
}

void gui_event_pfk(server_t *s, player_t *p)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "pfk #%d\n", p->id);
    gui_send(s, buf);
}

void gui_event_enw(server_t *s, int egg_id, player_t *p)
{
    char buf[128];

    snprintf(buf, sizeof(buf), "enw #%d #%d %d %d\n", egg_id, p->id, p->x, p->y);
    gui_send(s, buf);
}

void gui_event_edi(server_t *s, int egg_id)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "edi #%d\n", egg_id);
    gui_send(s, buf);
}

void gui_event_seg(server_t *s, const char *team)
{
    char buf[128];

    snprintf(buf, sizeof(buf),
        "seg %s\n", team);
    gui_send(s, buf);
}

void gui_event_eht(server_t *s, int egg_id)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "eht #%d\n", egg_id);
    send_to_gui(s, buf);
}

void gui_event_ebo(server_t *s, int egg_id)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "ebo #%d\n", egg_id);
    gui_send(s, buf);
}

void gui_event_pex(server_t *s, int player_id)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "pex #%d\n", player_id);
    gui_send(s, buf);
}
