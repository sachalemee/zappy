##
## EPITECH PROJECT, 2026
## zappy
## File description:
## navigator
##

from src.ai.strategy.actions import Action

# direction :
# 0 = North
# 1 = East
# 2 = South
# 3 = West
DELTAS = {
    0: (0, -1),
    1: (1, 0),
    2: (0, 1),
    3: (-1, 0),
}

class Navigator:
    @staticmethod
    def neighbors(pos: tuple[int, int], width: int, height: int) -> list[tuple[int, int]]:
        x, y = pos
        neighbors = []

        for dx, dy in DELTAS.values():
            neighbor_x = (x + dx) % width
            neighbor_y = (y + dy) % height
            neighbors.append((neighbor_x, neighbor_y))
        return neighbors

    @staticmethod
    def shortest_delta(delta: int, size: int) -> int:
        half_size = size // 2
        return (delta + half_size) % size - half_size
