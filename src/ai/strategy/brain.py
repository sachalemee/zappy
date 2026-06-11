##
## EPITECH PROJECT, 2026
## zappy
## File description:
## brain
##

from src.ai.game.state import GameState

from src.ai.strategy.actions import Action

from src.ai.algo.fsm import FSM, State
from src.ai.algo.requirements import missing_resources
from src.ai.algo.navigator import move_toward_ressource

class Brain:
    def __init__(self, state: GameState):
        self.state = state

    def decide(self) -> Action:
        pass
