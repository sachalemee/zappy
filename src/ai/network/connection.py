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
        self.buffer = ""
        self.closed_by_peer = False

    def connect(self) -> None:
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.machine, self.port))

    def send(self, line: str) -> None:
        if self.closed_by_peer:
            return
        try:
            self.socket.sendall((line + "\n").encode())
        except ConnectionError:
            self.closed_by_peer = True

    def receive_line(self) -> str | None:
        while "\n" not in self.buffer:
            try:
                chunk = self.socket.recv(4096).decode()
            except ConnectionError:
                chunk = ""
            if not chunk:
                self.closed_by_peer = True
                return None
            self.buffer += chunk
        line, self.buffer = self.buffer.split("\n", 1)
        return line.strip()

    def close(self) -> None:
        if self.socket:
            self.socket.close()
            self.socket = None
