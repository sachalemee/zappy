##
## EPITECH PROJECT, 2026
## zappy
## File description:
## main
##

import subprocess
import sys

from src.ai.cli import Confing_args, parse_args
from src.ai.logger import log
from src.ai.network.connection import Connection
from src.ai.network.protocol import Protocol
from src.ai.game.state import GameState
from src.ai.strategy.actions import Action
from src.ai.strategy.brain import Brain


def spawn_teammate(config: Confing_args) -> None:
    # egg hatching works like this : a new client just needs to connect to the team, the server randomly assigns it to a waiting egg
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
        # one command in flight at a time : brain reacts to the latest known state, so queuing more before a response comes back would make it repeat stale decisions (e.g. : turning multiple times for one right)
        if not protocol.pending:
            action = brain.decide()
            log(f"-> {action.to_command(brain.next_argument)}")
            protocol.send_command(action, brain.next_argument)

        for action, response in protocol.poll_responses():
            log(f"<- {response}")
            if response == "dead": # death works like this : no command, server just pushes this
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
            if action in (Action.FORWARD, Action.LEFT, Action.RIGHT, Action.EJECT):
                log(f"state: pos={state.position} dir={state.direction} level={state.level}")

        if state.pending_spawns > 0:
            state.pending_spawns -= 1
            spawn_teammate(config)
