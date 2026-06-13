##
## EPITECH PROJECT, 2026
## zappy
## File description:
## cli
##

from src.ai.helper import helper


class Config:
    def __init__(self, port: int, name: str, machine: str = "localhost"):
        self.port = port
        self.name = name
        self.machine = machine


def parse_args(argv: list[str]) -> Config:
    ## USAGE: ./zappy_ai -p port -n name -h machine
    ## Reads -p/-n/-h from args
    helper()
