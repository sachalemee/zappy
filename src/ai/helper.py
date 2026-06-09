##
## EPITECH PROJECT, 2026
## zappy
## File description:
## helper
##


IS_EXIT_FAILURE = 84
##  option                  description
##
##  -p port                 port number
##  -n name                 name of the team
##  -h machine              name of the machine; localhost by default
def helper():
    print("USAGE: ./zappy_ai -p port -n name -h machine")
    exit(IS_EXIT_FAILURE)
