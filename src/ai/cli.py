##
## EPITECH PROJECT, 2026
## zappy
## File description:
## cli
##

from dataclasses import dataclass

from src.ai.helper import helper


@dataclass
class Config:
    port: int
    name: str
    machine: str = "localhost"


def parse_args(argv: list[str]) -> Config:
    ## USAGE: ./zappy_ai -p port -n name -h machine
    ## Reads -p/-n/-h from args
    helper()
