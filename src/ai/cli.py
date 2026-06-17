##
## EPITECH PROJECT, 2026
## zappy
## File description:
## cli
##

import argparse

from src.ai.helper import helper


class Confing_args:
    def __init__(self, port: int, name: str, machine: str = "localhost"):
        self.port = port
        self.name = name
        self.machine = machine


class _Parser(argparse.ArgumentParser):
    def error(self, _message: str) -> None:
        helper()


def parse_args(argv: list[str]) -> Confing_args:
    parser = _Parser(add_help=False)
    parser.add_argument("-p", type=int, required=True, dest="port")
    parser.add_argument("-n", required=True, dest="name")
    parser.add_argument("-h", default="localhost", dest="machine")
    args = parser.parse_args(argv)
    return Confing_args(port=args.port, name=args.name, machine=args.machine)
