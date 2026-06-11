##
## EPITECH PROJECT, 2026
## zappy
## File description:
## actions
##

from enum import Enum

class Action(Enum):
    FORWARD = "Forward"
    RIGHT = "Right"
    LEFT = "Left"
    LOOK = "Look"
    INVENTORY = "Inventory"
    BROADCAST = "Broadcast"
    CONNECT_NBR = "Connect_nbr"
    FORK = "Fork"
    EJECT = "Eject"
    TAKE = "Take"
    SET = "Set"
    INCANTATION = "Incantation"

    def to_command(self, argument: str | None = None) -> str:
        if argument is None:
            return self.value
        return f"{self.value} {argument}"
