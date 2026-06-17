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

DIRECTION_DELTAS = {
    NORTH: (0, -1),
    EAST: (1, 0),
    SOUTH: (0, 1),
    WEST: (-1, 0),
}

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
        self.egg_slots = 0 # unhatched eggs available to the team, from the last Connect_nbr response

    def update_vision(self, tiles: list[Tile]) -> None:
        self.vision = tiles

    def update_inventory(self, inventory: Inventory) -> None:
        self.inventory = inventory

    def apply_turn(self, action: Action) -> None:
        if action == Action.RIGHT:
            self.direction = (self.direction + 1) % 4
        elif action == Action.LEFT:
            self.direction = (self.direction - 1) % 4

    def _move_in_direction(self, direction: int) -> None:
        x, y = self.position
        dx, dy = DIRECTION_DELTAS[direction]
        self.position = ((x + dx) % self.map_width, (y + dy) % self.map_height)

    def apply_move(self) -> None:
        self._move_in_direction(self.direction)

    def apply_eject(self, k: int) -> None:
        # k: relative push direction from the server ("eject: K"), same
        # octant convention as broadcast sound direction (1=front..8)
        if k in (8, 1, 2):
            relative = 0
        elif k in (3, 4):
            relative = 1
        elif k == 5:
            relative = 2
        else:
            relative = 3
        self._move_in_direction((self.direction + relative) % 4)

    def current_tile(self) -> Tile | None:
        return self.vision[0] if self.vision else None
