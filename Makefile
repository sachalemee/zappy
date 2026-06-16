##
## EPITECH PROJECT, 2026
## zappy
## File description:
## Makefile
##


## SERVER
SERVER_SRC = $(addprefix src/server/, \
                main.c \
                parsing/helper.c \
                parsing/error_handling.c \
                parsing/parse_args.c \
                parsing/parse_teams.c \
                network/accept_client.c \
                network/bind_socket.c \
                network/client_buffer.c \
                network/create_socket.c \
                network/destroy_server.c \
                network/handshake.c \
                network/init_server.c \
                network/init_map.c \
                network/listen_socket.c \
                network/poll_loop.c \
                network/refill_map.c \
                network/time_utils.c \
                network/timeout.c \
                network/famine.c    \
                team/find_team.c \
                player/add_player.c \
                player/create_player.c \
                commands/execute_command.c \
                commands/forward.c \
                commands/left.c \
                commands/right.c \
                commands/look.c \
                commands/inventory.c \
                protocol/protocol.c \
                gui/gui_broadcast.c \
                gui/gui_cmd.c   \
                gui/gui_event.c \
                gui/gui_player.c \
            )

SERVER_NAME		= zappy_server
SERVER_OBJ 		= $(SERVER_SRC:src/server/%.c=obj/server/%.o)
SERVER_OBJ_DIR	= obj/server

## GUI
GUI_SRC         = $(addprefix src/gui/, \
                    main.cpp \
                    error_handling.cpp \
                    helper.cpp \
                    Window.cpp \
                    menu_gui/Menu.cpp \
                    menu_gui/TextBox.cpp \
                    settings.cpp \
                    network/network_client.cpp \
                    application.cpp \
                    gamecamera.cpp \
                    renderer.cpp \
                    game/gamestate.cpp \
                )

GUI_NAME		= zappy_gui
GUI_OBJ			= $(addprefix obj/gui/, $(notdir $(GUI_SRC:.cpp=.o)))
GUI_OBJ_DIR		= obj/gui

## AI
CP_R 			= cp -r

AI_NAME			= zappy_ai

AI_FILE_EXT		= .py

AI_PATH			= $(addprefix src/ai/, $(AI_NAME)$(AI_FILE_EXT))

## Makefile rules definition
CC				?= clang
CXX				?= clang++
MV				?= mv
RM				?= rm -f

CFLAGS			+= -Wall -Wextra -Werror
CXXFLAGS		+= -Wall -Wextra -Werror -std=c++20

CPPFLAGS		+= -I include/

SERVER_CPPFLAGS	+= -I include/server/
GUI_CPPFLAGS	+= -I include/gui/

SERVER_LDFLAGS	+= -lpthread

GUI_LDFLAGS     += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

## Makefile compilation rules definition
all:			zappy_server zappy_gui zappy_ai

$(SERVER_OBJ_DIR):
				mkdir -p $(SERVER_OBJ_DIR)

$(GUI_OBJ_DIR):
				mkdir -p $(GUI_OBJ_DIR)

zappy_server:	$(SERVER_OBJ_DIR) $(SERVER_OBJ)
				$(CC) -o $(SERVER_NAME) $(SERVER_OBJ) $(SERVER_LDFLAGS)

zappy_gui:		$(GUI_OBJ_DIR) $(GUI_OBJ)
				$(CXX) -o $(GUI_NAME) $(GUI_OBJ) $(GUI_LDFLAGS)

zappy_ai:		
				$(CP_R) $(AI_PATH) .
				$(MV) $(AI_NAME)$(AI_FILE_EXT) $(AI_NAME)

## Compilation rules for object files
obj/server/%.o: src/server/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SERVER_CPPFLAGS) -c $< -o $@

obj/gui/%.o:	src/gui/%.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(GUI_CPPFLAGS) -c $< -o $@

obj/gui/Menu.o:		src/gui/menu_gui/Menu.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(GUI_CPPFLAGS) -c $< -o $@

obj/gui/TextBox.o:	src/gui/menu_gui/TextBox.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(GUI_CPPFLAGS) -c $< -o $@

obj/gui/network_client.o:    src/gui/network/network_client.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(GUI_CPPFLAGS) -c $< -o $@

obj/gui/gamestate.o:    src/gui/game/gamestate.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(GUI_CPPFLAGS) -c $< -o $@

clean:
				$(RM) -r $(SERVER_OBJ_DIR) $(GUI_OBJ_DIR)

fclean:			clean
				$(RM) $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)

re:				fclean all

.PHONY:			all clean fclean re
