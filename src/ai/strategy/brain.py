##
## EPITECH PROJECT, 2026
## zappy
## File description:
## brain
##

from src.ai.game.state import GameState
from src.ai.strategy.actions import Action


class Brain:
    def __init__(self, state: GameState):
        self.state = state

    def decide(self) -> Action:
        ## Rule-based first, swappable for an ML-backed decision later (see ml/model.py)
        pass
