/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** command_table
*/

#include "zappy.h"
#include <string.h>

int get_command_delay(char *cmd, int freq)
{
    if (strcmp(cmd, "Forward") == 0)
        return 7 / freq;
    if (strcmp(cmd, "Right") == 0)
        return 7 / freq;
    if (strcmp(cmd, "Left") == 0)
        return 7 / freq;
    if (strcmp(cmd, "Look") == 0)
        return 7 / freq;
    if (strcmp(cmd, "Inventory") == 0)
        return 1 / freq;
    return -1;
}
