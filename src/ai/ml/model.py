##
## EPITECH PROJECT, 2026
## zappy
## File description:
## model
##

from src.ai.game.state import GameState
from src.ai.strategy.actions import Action

## Wraps the PyTorch/TensorFlow network so the rest of the AI never
## depends directly on either framework.


class Model:
    def __init__(self, weights_path: str | None = None):
        self.weights_path = weights_path
        self.network = None

    def load(self, path: str) -> None:
        pass

    def predict(self, state: GameState) -> Action:
        pass
