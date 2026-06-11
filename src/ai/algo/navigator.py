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
    def __init__(self):
        self.visited: set[tuple[int, int]] = set()
        self.stack: list[tuple[int, int]] = []

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

    def next_target(self, game_state) -> tuple[int, int]:
        position = game_state.position

        self.visited.add(position)

        width = game_state.map_width
        height = game_state.map_height

        neighbors = self.neighbors(position, width, height)

        for neighbor in neighbors:
            if neighbor not in self.visited:
                self.stack.append(position)
                return neighbor

        while self.stack:
            previous = self.stack.pop()

            neighbors = self.neighbors(previous, width, height)

            for neighbor in neighbors:
                if neighbor not in self.visited:
                    self.stack.append(previous)
                    return neighbor
        return position

    def move_toward_ressource(self, game_state) -> Action:
        target = self.next_target(game_state)
        if target == game_state.position:
            return Action.FORWARD
        return Action.LEFT
    