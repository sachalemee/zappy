/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** commands
*/

#ifndef COMMANDS_H_
    #define COMMANDS_H_

#include "zappy.h"

void cmd_forward(server_t *s, client_t *c);
void cmd_right(server_t *s, client_t *c);
void cmd_left(server_t *s, client_t *c);
void cmd_look(server_t *s, client_t *c);
void cmd_inventory(server_t *s, client_t *c);

#endif
