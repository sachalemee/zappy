##
## EPITECH PROJECT, 2026
## zappy
## File description:
## state
##

from src.ai.game.inventory import Inventory
from src.ai.game.tile import Tile


class GameState:
    def __init__(self, map_width: int, map_height: int):
        self.map_width = map_width
        self.map_height = map_height
        self.orientation = 0
        self.level = 1
        self.inventory = Inventory()
        self.vision: list[Tile] = []

    def update_vision(self, tiles: list[Tile]) -> None:
        self.vision = tiles

    def update_inventory(self, inventory: Inventory) -> None:
        self.inventory = inventory
