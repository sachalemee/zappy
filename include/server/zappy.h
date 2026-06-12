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
    STATE_WAIT_TEAM,
    STATE_IN_GAME
} client_state_t;

typedef enum client_type_e {
    CLIENT_AI,
    CLIENT_GUI
} client_type_t;

typedef struct command_s {
    char *name;
    int delay;
    struct command_s *next;
} command_t;

typedef struct client_s {
    int fd;

    client_state_t state;
    client_type_t type;

    char buffer[1024];
    int buf_len;

    server_t *server;
    team_t *team;
    player_t *player;

    command_t *cmd_queue;
    int cmd_busy;
} client_t;

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

//COMMANDE

void push_command(client_t *c, char *name, int delay);
int get_command_delay(char *cmd, int freq);
void process_commands(client_t *c);


//TEAM

team_t *find_team(server_t *s, const char *name);

//PLAYER

player_t *create_player(server_t *s, team_t *team);
int add_player(server_t *s, player_t *p);

#endif
