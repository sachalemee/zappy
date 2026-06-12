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
void cmd_right(client_t *c);
void cmd_left(client_t *c);
void cmd_look(client_t *c);
void cmd_inventory(client_t *c);
void process_commands(client_t *c);

#endif
