##
## EPITECH PROJECT, 2026
## zappy
## File description:
## protocol
##

import socket
from collections import deque

from src.ai.network.connection import Connection
from src.ai.strategy.actions import Action

## Lines pushed by the server outside of the request/response flow
_SERVER_PUSH_PREFIXES = ("message ", "dead", "eject:", "Elevation underway")


class Protocol:
    def __init__(self, connection: Connection):
        self.connection = connection
        self.pending: deque[Action] = deque()

    def handshake(self, team_name: str) -> tuple[int, int]:
        welcome = self.connection.receive_line()
        if welcome != "WELCOME":
            raise ConnectionError(f"Expected WELCOME, got: {welcome}")
        self.connection.send(team_name)
        client_num = self.connection.receive_line() # need to implement when game state done
        if client_num is None:
            raise ConnectionError(
                f"Server closed the connection right after sending team name '{team_name}' "
                "(no slot available, or unknown team name)"
            )
        size_line = self.connection.receive_line()
        if size_line is None:
            raise ConnectionError("Server closed the connection before sending the map size")
        width, height = size_line.split()
        return int(width), int(height)

    def send_command(self, action: Action, argument: str | None = None) -> None:
        self.connection.send(action.to_command(argument))
        self.pending.append(action)

    def poll_responses(self) -> list[tuple[Action | None, str]]:
        results = []
        self.connection.socket.settimeout(0.05)
        try:
            while True:
                line = self.connection.receive_line()
                if line is None:
                    break
                if any(line.startswith(p) for p in _SERVER_PUSH_PREFIXES):
                    results.append((None, line))
                    continue
                if line.startswith("Current level:") and (
                    not self.pending or self.pending[0] != Action.INCANTATION
                ):
                    results.append((None, line))
                    continue
                if self.pending:
                    results.append((self.pending.popleft(), line))
        except socket.timeout:
            pass
        finally:
            self.connection.socket.settimeout(None)
        return results
