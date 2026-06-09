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
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.machine, self.port))

    def send(self, line: str) -> None:
        self.socket.sendall((line + "\n").encode())

    def receive_line(self) -> str | None:
        while "\n" not in self._buffer:
            chunk = self.socket.recv(4096).decode()
            if not chunk:
                return None
            self._buffer += chunk
        line, self._buffer = self._buffer.split("\n", 1)
        return line.strip()

    def close(self) -> None:
        if self.socket:
            self.socket.close()
            self.socket = None
