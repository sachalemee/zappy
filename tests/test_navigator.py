##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_navigator
##

import unittest

from src.ai.algo.navigator import Navigator
from src.ai.game.state import GameState, NORTH, EAST, SOUTH
from src.ai.strategy.actions import Action
from tests.colors import ColorTextTestRunner


class TestNeighbors(unittest.TestCase):
    def test_default_order_is_north_east_south_west(self):
        nav = Navigator(start_offset=0)
        # from (5,5) on a 10x10 map : N=(5,4) E=(6,5) S=(5,6) W=(4,5)
        self.assertEqual(nav.neighbors((5, 5), 10, 10), [(5, 4), (6, 5), (5, 6), (4, 5)])

    def test_offset_rotates_the_order(self):
        nav = Navigator(start_offset=2)
        # offset 2 starts at South instead of North, same cycle otherwise
        self.assertEqual(nav.neighbors((5, 5), 10, 10), [(5, 6), (4, 5), (5, 4), (6, 5)])

    def test_wraps_around_map_edges(self):
        nav = Navigator(start_offset=0)
        # at (0,0) on a 10x10 map : North wraps to y=9, West wraps to x=9
        self.assertEqual(nav.neighbors((0, 0), 10, 10), [(0, 9), (1, 0), (0, 1), (9, 0)])


class TestShortestDelta(unittest.TestCase):
    def test_no_wraparound_needed(self):
        self.assertEqual(Navigator.shortest_delta(3, 10), 3)
        self.assertEqual(Navigator.shortest_delta(-3, 10), -3)

    def test_wraps_around_when_shorter(self):
        # going from x=1 to x=8 on a width-10 map : direct delta is 7, but going
        # the other way (through the edge) is only -3
        self.assertEqual(Navigator.shortest_delta(7, 10), -3)
        self.assertEqual(Navigator.shortest_delta(-7, 10), 3)

    def test_exact_half_map(self):
        self.assertEqual(Navigator.shortest_delta(5, 10), -5)


class TestNextTarget(unittest.TestCase):
    def _walk(self, nav: Navigator, state: GameState, steps: int) -> list[tuple[int, int]]:
        targets = []
        for _ in range(steps):
            target = nav.next_target(state)
            targets.append(target)
            state.position = target # simulate actually moving there, like Brain would
        return targets

    def test_first_call_visits_an_unvisited_neighbor(self):
        nav = Navigator(start_offset=0)
        state = GameState(3, 3, "team1")
        state.position = (1, 1)
        target = nav.next_target(state)
        self.assertEqual(target, (1, 0)) # North, per the default exploration order
        self.assertIn((1, 1), nav.visited)
        self.assertEqual(nav.stack, [(1, 1)])

    def test_eventually_covers_every_tile_before_resetting(self):
        nav = Navigator(start_offset=0)
        state = GameState(3, 3, "team1")
        state.position = (1, 1)

        targets = self._walk(nav, state, 8) # 3x3 map minus the starting tile = 8 new tiles
        visited_tiles = {(1, 1)} | set(targets)
        self.assertEqual(visited_tiles, {(x, y) for x in range(3) for y in range(3)})

    def test_resets_and_stays_put_once_everything_is_visited(self):
        nav = Navigator(start_offset=0)
        state = GameState(3, 3, "team1")
        state.position = (1, 1)
        self._walk(nav, state, 8) # cover the whole map

        before_reset = state.position
        target = nav.next_target(state)
        self.assertEqual(target, before_reset) # nothing left to explore ; stays put
        self.assertEqual(nav.visited, set())
        self.assertEqual(nav.stack, [])

    def test_starts_a_fresh_cycle_after_resetting(self):
        nav = Navigator(start_offset=0)
        state = GameState(3, 3, "team1")
        state.position = (1, 1)
        self._walk(nav, state, 9) # full cycle + the reset call

        target = nav.next_target(state)
        self.assertNotEqual(target, state.position) # exploring again, not stuck


class TestMoveTowardRessource(unittest.TestCase):
    def _move(self, nav: Navigator, state: GameState, target: tuple[int, int]) -> Action:
        nav.next_target = lambda _state: target # isolate the turning logic from the DFS
        return nav.move_toward_ressource(state)

    def test_forward_when_target_is_current_position(self):
        nav = Navigator(start_offset=0)
        state = GameState(10, 10, "team1")
        state.position = (5, 5)
        state.direction = NORTH
        self.assertEqual(self._move(nav, state, (5, 5)), Action.FORWARD)

    def test_forward_when_already_facing_the_target(self):
        nav = Navigator(start_offset=0)
        state = GameState(10, 10, "team1")
        state.position = (5, 5)
        state.direction = NORTH
        self.assertEqual(self._move(nav, state, (5, 4)), Action.FORWARD)

    def test_right_when_target_is_one_clockwise_turn_away(self):
        nav = Navigator(start_offset=0)
        state = GameState(10, 10, "team1")
        state.position = (5, 5)
        state.direction = NORTH
        self.assertEqual(self._move(nav, state, (6, 5)), Action.RIGHT) # East

    def test_left_otherwise(self):
        nav = Navigator(start_offset=0)
        state = GameState(10, 10, "team1")
        state.position = (5, 5)
        state.direction = EAST
        self.assertEqual(self._move(nav, state, (5, 4)), Action.LEFT) # North

    def test_left_when_target_is_directly_behind(self):
        nav = Navigator(start_offset=0)
        state = GameState(10, 10, "team1")
        state.position = (5, 5)
        state.direction = SOUTH
        self.assertEqual(self._move(nav, state, (5, 4)), Action.LEFT) # North, behind us


class TestMoveTowardPosition(unittest.TestCase):
    def test_forward_when_target_is_current_position(self):
        state = GameState(10, 10, "team1")
        state.position = (5, 5)
        state.direction = NORTH
        self.assertEqual(Navigator.move_toward_position(state, (5, 5)), Action.FORWARD)

    def test_turns_toward_an_absolute_target(self):
        state = GameState(10, 10, "team1")
        state.position = (5, 5)
        state.direction = NORTH
        self.assertEqual(Navigator.move_toward_position(state, (6, 5)), Action.RIGHT) # East

    def test_wraps_around_map_edges(self):
        state = GameState(10, 10, "team1")
        state.position = (0, 0)
        state.direction = EAST
        # shortest path to x=9 on a width-10 map is West (wraparound), not East
        self.assertEqual(Navigator.move_toward_position(state, (9, 0)), Action.LEFT)


if __name__ == "__main__":
    unittest.main(testRunner=ColorTextTestRunner)
