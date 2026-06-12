##
## EPITECH PROJECT, 2026
## zappy
## File description:
## state
##

from src.ai.game.inventory import Inventory
from src.ai.game.tile import Tile
from src.ai.strategy.actions import Action

# direction :
# 0 = North
# 1 = East
# 2 = South
# 3 = West
NORTH, EAST, SOUTH, WEST = range(4)

class GameState:
    def __init__(self, map_width: int, map_height: int, team_name: str = ""):
        self.map_width = map_width
        self.map_height = map_height
        self.team_name = team_name
        self.position = (0, 0)
        self.direction = NORTH
        self.level = 1
        self.inventory = Inventory()
        self.vision: list[Tile] = []
        self.incoming_call: tuple[int, int] | None = None # (level, k) of the last incantation call heard (FROM TUNG TUNG SAHUR) from a teammate at our level or none

    def update_vision(self, tiles: list[Tile]) -> None:
        self.vision = tiles

    def update_inventory(self, inventory: Inventory) -> None:
        self.inventory = inventory

    def apply_turn(self, action: Action) -> None:
        if action == Action.RIGHT:
            self.direction = (self.direction + 1) % 4
        elif action == Action.LEFT:
            self.direction = (self.direction - 1) % 4

    def apply_move(self) -> None:
        x, y = self.position
        if self.direction == NORTH:
            y = (y - 1) % self.map_height
        elif self.direction == EAST:
            x = (x + 1) % self.map_width
        elif self.direction == SOUTH:
            y = (y + 1) % self.map_height
        elif self.direction == WEST:
            x = (x - 1) % self.map_width
        self.position = (x, y)

    def current_tile(self) -> Tile | None:
        return self.vision[0] if self.vision else None
