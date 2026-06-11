##
## EPITECH PROJECT, 2026
## zappy
## File description:
## fsm
##

from enum import Enum, auto

class State(Enum):
    SURVIVE = auto()
    COLLECT = auto()
    BROADCAST = auto()
    INCANTATE = auto()
    FORK = auto()

# Finite State Machine
class FSM:
    def __init__(self):
        pass
    
    def ready_to_incantate(self):
        pass
    
    def transition(self):
        pass
