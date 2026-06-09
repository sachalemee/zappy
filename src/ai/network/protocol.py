##
## EPITECH PROJECT, 2026
## zappy
## File description:
## protocol
##

from collections import deque
from src.ai.network.connection import Connection
from src.ai.strategy.actions import Action

MAX_PENDING_COMMANDS = 10


class Protocol:
    def __init__(self, connection: Connection):
        self.connection = connection
        self.pending: deque[Action] = deque()

    def handshake(self, team_name: str) -> tuple[int, int]:
        ## WELCOME -> TEAM-NAME -> CLIENT-NUM -> X Y
        pass

    def can_send(self) -> bool:
        return len(self.pending) < MAX_PENDING_COMMANDS

    def send_command(self, action: Action) -> None:
        pass

    def poll_responses(self) -> list[str]:
        pass
