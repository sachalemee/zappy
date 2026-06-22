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
    def __init__(self, start_offset: int = 0):
        self.visited: set[tuple[int, int]] = set()
        self.stack: list[tuple[int, int]] = []
        self._direction_order = [(start_offset + i) % 4 for i in range(4)]

    def neighbors(self, pos: tuple[int, int], width: int, height: int) -> list[tuple[int, int]]:
        x, y = pos
        neighbors = []

        for direction in self._direction_order:
            dx, dy = DELTAS[direction]
            neighbor_x = (x + dx) % width
            neighbor_y = (y + dy) % height
            neighbors.append((neighbor_x, neighbor_y))
        return neighbors

    @staticmethod
    def shortest_delta(delta: int, size: int) -> int:
        # the map wraps like this : going off one edge re-enters on the other, so the shortest path may cross the wraparound point
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

        self.visited.clear()
        self.stack.clear()
        return position

    def move_toward_ressource(self, game_state) -> Action:
        target = self.next_target(game_state)

        if target == game_state.position:
            return Action.FORWARD

        x, y = game_state.position
        tx, ty = target

        dx = self.shortest_delta(tx - x, game_state.map_width)
        dy = self.shortest_delta(ty - y, game_state.map_height)

        if dx > 0:
            wanted_direction = 1
        elif dx < 0:
            wanted_direction = 3
        elif dy < 0:
            wanted_direction = 0
        else:
            wanted_direction = 2
        if wanted_direction == game_state.direction:
            return Action.FORWARD
        if (game_state.direction + 1) % 4 == wanted_direction:
            return Action.RIGHT
        return Action.LEFT

    @staticmethod
    def move_toward_sound(k: int) -> Action:
        # broadcast sound works like this : k=0 means we're on the emitter's tile, k=1 is straight ahead, k=2..8 circle around the player; assumes clockwise ordering (no diagram to confirm trigonometric dir)
        if k == 1:
            return Action.FORWARD
        if k in (2, 3, 4, 5):
            return Action.RIGHT
        return Action.LEFT
