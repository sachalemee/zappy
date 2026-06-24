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
        self.incoming_call: tuple[int, int] | None = None # last incantation call heard (level, k), if any
        self.known_free_slots = 0 # team's free egg slots, as last reported by the server via Connect_nbr
        self.known_resources: dict[str, tuple[int, int]] = {} # resource -> absolute position last reported by a sibling

    def update_vision(self, tiles: list[Tile]) -> None:
        self.vision = tiles

    def update_inventory(self, inventory: Inventory) -> None:
        self.inventory = inventory

    def apply_turn(self, action: Action) -> None:
        if action == Action.RIGHT:
            self.direction = (self.direction + 1) % 4
        elif action == Action.LEFT:
            self.direction = (self.direction - 1) % 4

    def move_in_direction(self, direction: int) -> None:
        # the map works like this : it's toroidal, exiting one edge wraps around to the opposite edge (modulo width/height)
        x, y = self.position
        dx, dy = DIRECTION_DELTAS[direction]
        self.position = ((x + dx) % self.map_width, (y + dy) % self.map_height)

    def apply_move(self) -> None:
        self.move_in_direction(self.direction)

    def apply_eject(self, k: int) -> None:
        # eject works like this : k=5 means we got pushed forward, then clockwise from there (k=3 right, k=1 behind, k=7 left)
        if k == 5:
            relative = 0
        elif k == 3:
            relative = 1
        elif k == 1:
            relative = 2
        else:
            relative = 3
        self.move_in_direction((self.direction + relative) % 4)

    def current_tile(self) -> Tile | None:
        # vision works like this : Look returns one tile per visible cell, vision[0] is always our own tile regardless of level/direction
        return self.vision[0] if self.vision else None
