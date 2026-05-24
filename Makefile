##
## EPITECH PROJECT, 2026
## zappy
## File description:
## Makefile
##


## SERVER
SERVER_SRC		= $(addprefix src/server/, \
					main.c \
				)

SERVER_NAME		= zappy_server
SERVER_OBJ		= $(SERVER_SRC:.c=.o)

## GUI
GUI_SRC			= $(addprefix src/gui/, \
					main.cpp \
				)

GUI_NAME		= zappy_gui
GUI_OBJ			= $(GUI_SRC:.cpp=.o)

## AI
AI_SRC			= $(addprefix src/ai/, \
					main.cpp \
				)

AI_NAME			= zappy_ai
AI_OBJ			= $(AI_SRC:.cpp=.o)

CC				?= clang
CXX				?= clang++

CFLAGS			+= -Wall -Wextra -Werror
CXXFLAGS		+= -Wall -Wextra -Werror -std=c++20

CPPFLAGS		+= -I include/

SERVER_LDFLAGS	+= -lpthread

GUI_LDFLAGS		+= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

AI_LDFLAGS		+=

all:			zappy_server zappy_gui zappy_ai

zappy_server:	$(SERVER_OBJ)
				$(CC) -o $(SERVER_NAME) $(SERVER_OBJ) $(SERVER_LDFLAGS)

zappy_gui:		$(GUI_OBJ)
				$(CXX) -o $(GUI_NAME) $(GUI_OBJ) $(GUI_LDFLAGS)

zappy_ai:		$(AI_OBJ)
				$(CXX) -o $(AI_NAME) $(AI_OBJ) $(AI_LDFLAGS)

%.o:			%.c
				$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o:			%.cpp
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
				$(RM) $(SERVER_OBJ) $(GUI_OBJ) $(AI_OBJ)

fclean:			clean
				$(RM) $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)

re:				fclean all

.PHONY:			all zappy_server zappy_gui zappy_ai clean fclean re
