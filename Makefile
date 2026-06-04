##
## EPITECH PROJECT, 2026
## zappy
## File description:
## Makefile
##


## SERVER
SERVER_SRC		= $(addprefix src/server/, \
					main.c \
					error_handling.c \
					helper.c \
				)

SERVER_NAME		= zappy_server
SERVER_OBJ		= $(addprefix obj/server/, $(notdir $(SERVER_SRC:.c=.o)))
SERVER_OBJ_DIR		= obj/server

## GUI
GUI_SRC			= $(addprefix src/gui/, \
					main.cpp \
					error_handling.cpp \
					helper.cpp \
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

GUI_LDFLAGS		+= -lraylib

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
obj/server/%.o:	src/server/%.c
				$(CC) $(CFLAGS) $(CPPFLAGS) $(SERVER_CPPFLAGS) -c $< -o $@

obj/gui/%.o:	src/gui/%.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(GUI_CPPFLAGS) -c $< -o $@

clean:
				$(RM) -r $(SERVER_OBJ_DIR) $(GUI_OBJ_DIR)

fclean:			clean
				$(RM) $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)

re:				fclean all

.PHONY:			all clean fclean re
