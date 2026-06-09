##
## EPITECH PROJECT, 2026
## zappy
## File description:
## connection
##

import socket


class Connection:
    def __init__(self, machine: str, port: int):
        self.machine = machine
        self.port = port
        self.socket: socket.socket | None = None
        self._buffer = ""

    def connect(self) -> None:
        pass

    def send(self, line: str) -> None:
        pass

    def receive_line(self) -> str | None:
        pass

    def close(self) -> None:
        pass
