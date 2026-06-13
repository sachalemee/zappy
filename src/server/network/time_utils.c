/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** time_utils
*/

#include <sys/time.h>
#include <stddef.h>

long long get_current_time(void)
{
    struct timeval tv;
    long long time_in_ms;

    gettimeofday(&tv, NULL);
    time_in_ms = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
    return time_in_ms;
}

long long get_action_duration(int freq, int action_cost)
{
    if (freq <= 0)
        return 0;
    return ((long long)action_cost * 1000LL) / freq;
}
