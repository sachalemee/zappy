##
## EPITECH PROJECT, 2026
## zappy
## File description:
## main
##

import sys

from src.ai.cli import parse_args
from src.ai.network.connection import Connection
from src.ai.network.protocol import Protocol
from src.ai.game.state import GameState
from src.ai.strategy.brain import Brain


def main(argv: list[str] = sys.argv) -> None:
    config = parse_args(argv[1:])
    connection = Connection(config.machine, config.port)
    protocol = Protocol(connection)

    connection.connect()
    width, height = protocol.handshake(config.name)
    state = GameState(width, height)
    brain = Brain(state)

    while True:
        if protocol.can_send():
            protocol.send_command(brain.decide())
        protocol.poll_responses()
