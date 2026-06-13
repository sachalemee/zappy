/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** zappy
*/

#ifndef ZAPPY_H_
    #define ZAPPY_H_

#define IS_EXIT_FAILURE 84
#define IS_EXIT_SUCCESS 0
#define MAX_CMDS 10
#define MAX_CMD_LEN 1024

typedef struct server_s server_t;

typedef enum resource_e {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    RESOURCE_MAX
} resource_t;

typedef struct tile_s {
    int resources[RESOURCE_MAX];
} tile_t;

typedef struct map_s {
    int width;
    int height;
    tile_t **tiles;
} map_t;

typedef struct team_s {
    char *name;
    int free_slots;
} team_t;

typedef struct player_s {
    int fd;
    int id;
    int x;
    int y;
    int level;
    int orientation;
    int inventory[RESOURCE_MAX];
    team_t *team;
} player_t;

typedef enum client_state_e {
    STATE_WELCOME,
    STATE_TEAM,
    STATE_IN_GAME
} client_state_t;

typedef struct client_s {
    int fd;
    client_state_t state;
    char buffer[MAX_CMD_LEN];
    int buf_len;
    char cmds[MAX_CMDS][MAX_CMD_LEN];
    int nb_cmds;
    long long action_end_time;
    server_t *server;
    team_t *team;
    player_t *player;
} client_t;

//SERVER CORE

typedef struct server_s {
    int port;
    int width;
    int height;
    int freq;
    int clients_nb;

    int server_fd;

    map_t map;

    team_t *teams;
    int nb_teams;

    player_t **players;
    int nb_players;

    client_t **clients;
    int nb_clients;
} server_t;

//SERVER NETWORK

int create_socket(server_t *s);
int bind_socket(server_t *s);
int listen_socket(server_t *s);
int run_server(server_t *s);
int accept_client(server_t *s);

//PARSING

int error_handling(int ac, char **av, server_t *s);
int parse_args(int ac, char **av, server_t *s);
char **parse_teams(char **av, int *i);
int helper(void);

//SERVER

int init_server(server_t *s);
void destroy_server(server_t *s);
int init_map(server_t *s);
void refill_map(server_t *s);
void extract_commands(client_t *c);
void handle_handshake(client_t *c);
long long get_current_time(void);
long long get_action_duration(int freq, int action_cost);

#endif
