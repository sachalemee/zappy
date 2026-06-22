##
## EPITECH PROJECT, 2026
## zappy
## File description:
## main
##

import subprocess
import sys

from src.ai.cli import Confing_args, parse_args
from src.ai.network.connection import Connection
from src.ai.network.protocol import Protocol
from src.ai.game.state import GameState
from src.ai.strategy.brain import Brain


def spawn_teammate(config: Confing_args) -> None:
    subprocess.Popen([
        sys.executable, "-m", "src.ai.main",
        "-p", str(config.port), "-n", config.name, "-h", config.machine,
    ])


def main(argv: list[str] = sys.argv) -> None:
    config = parse_args(argv[1:])
    connection = Connection(config.machine, config.port)
    protocol = Protocol(connection)

    connection.connect()
    width, height = protocol.handshake(config.name)
    state = GameState(width, height, config.name)
    brain = Brain(state)

    while True:
        # one command in flight at a time : brain reacts to the latest known state, so queuing more before a response comes back would make it repeat stale decisions (e.g. turning multiple times for one right)
        if not protocol.pending:
            action = brain.decide()
            protocol.send_command(action, brain.next_argument)

        for action, response in protocol.poll_responses():
            if response == "dead":
                connection.close()
                return
            if action is None:
                if brain.handle_broadcast(response):
                    continue
                if brain.handle_eject(response):
                    continue
                if brain.handle_level_up(response):
                    continue
                continue
            brain.update(action, response)

        if state.egg_slots > 0:
            state.egg_slots -= 1
            spawn_teammate(config)
