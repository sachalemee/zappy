##
## EPITECH PROJECT, 2026
## zappy
## File description:
## logger
##

import os

_TAG = f"[AI #{os.getpid()}]"


def log(message: str) -> None:
    print(f"{_TAG} {message}", flush=True)
