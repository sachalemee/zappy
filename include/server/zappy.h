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
    long long next_meal_time;
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
    char buffer[MAX_CMD_LEN];
    int buf_len;
    char cmds[MAX_CMDS][MAX_CMD_LEN];
    int nb_cmds;
    long long action_end_time;
    server_t *server;
    team_t *team;
    player_t *player;
} client_t;

typedef struct egg_s {
    int id;
    int player_id;
    int x;
    int y;
    team_t *team;
    long long hatch_time;
    int hatched;
    long long death_time;
} egg_t;

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
    egg_t **eggs;
    int nb_eggs;
    long long next_refill_time;
    int next_egg_id;
    int next_player_id;
} server_t;

typedef int (*cmd_pre_func_t)(server_t *, client_t *);
typedef void (*cmd_func_t)(server_t *, client_t *);

typedef struct cmd_map_s {
    char *name;
    int time_cost;
    cmd_pre_func_t pre_func;
    cmd_func_t func;
} cmd_map_t;

//SERVER NETWORK

int create_socket(server_t *s);
int bind_socket(server_t *s);
int listen_socket(server_t *s);
int run_server(server_t *s);
int accept_client(server_t *s);
void send_to_gui(server_t *s, const char *msg);

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
int calculate_timeout(server_t *s);
void handle_active_commands(server_t *s);
void pop_command(client_t *c);
void cmd_forward(server_t *s, client_t *c);
void cmd_right(server_t *s, client_t *c);
void cmd_left(server_t *s, client_t *c);
void handle_famine(server_t *s);
void handle_eggs(server_t *s);
void gui_event_eht(server_t *s, int egg_id);
void consume_egg(server_t *s, team_t *t, player_t *p);
int check_win(server_t *s);
void generate_initial_eggs(server_t *s);
void gui_send_initial_data(server_t *s, int fd);

//COMMANDE

void handle_gui_command(server_t *s, client_t *c);
void handle_gui_player_command(server_t *s, client_t *c);

//TEAM

team_t *find_team(server_t *s, const char *name);

//PLAYER

player_t *create_player(server_t *s, team_t *team);
int add_player(server_t *s, player_t *p);

//GUI

void gui_event_bct(server_t *s, int x, int y);
void gui_event_mct(server_t *s);
void gui_event_pnw(server_t *s, player_t *p);
void gui_event_ppo(server_t *s, player_t *p);
void gui_event_pin(server_t *s, player_t *p);
void gui_event_plv(server_t *s, player_t *p);
void gui_event_pgt(server_t *s, player_t *p, int res);
void gui_event_pdr(server_t *s, player_t *p, int res);
void gui_event_pbc(server_t *s, player_t *p, const char *msg);
void gui_event_pic(server_t *s, player_t *p);
void gui_event_pie(server_t *s, player_t *p, int result);
void gui_event_pfk(server_t *s, player_t *p);
void gui_event_enw(server_t *s, int egg_id, player_t *p);
void gui_event_edi(server_t *s, int egg_id);
void gui_event_seg(server_t *s, const char *team);
void gui_event_ebo(server_t *s, int egg_id);


//PROTOCOL

void gui_send_ppo(server_t *s, player_t *p);
void gui_send_bct(server_t *s, int x, int y);
void gui_send_mct(server_t *s);

//EGG

egg_t *spawn_egg(server_t *s, client_t *c);
void gui_event_pex(server_t *s, int player_id);
void destroy_egg(server_t *s, int egg_id);

#endif
