##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_state
##

import unittest

from src.ai.game.state import GameState, NORTH, EAST, SOUTH, WEST
from src.ai.game.tile import Tile
from src.ai.strategy.actions import Action


class TestApplyTurn(unittest.TestCase):
    def test_right_turns_clockwise(self):
        state = GameState(10, 10)
        state.direction = NORTH
        state.apply_turn(Action.RIGHT)
        self.assertEqual(state.direction, EAST)

    def test_left_turns_counter_clockwise(self):
        state = GameState(10, 10)
        state.direction = NORTH
        state.apply_turn(Action.LEFT)
        self.assertEqual(state.direction, WEST)

    def test_right_wraps_around(self):
        state = GameState(10, 10)
        state.direction = WEST
        state.apply_turn(Action.RIGHT)
        self.assertEqual(state.direction, NORTH)

    def test_unrelated_action_does_not_turn(self):
        state = GameState(10, 10)
        state.direction = NORTH
        state.apply_turn(Action.FORWARD)
        self.assertEqual(state.direction, NORTH)


class TestMoveInDirection(unittest.TestCase):
    def test_moves_north(self):
        state = GameState(10, 10)
        state.position = (5, 5)
        state.move_in_direction(NORTH)
        self.assertEqual(state.position, (5, 4))

    def test_moves_east(self):
        state = GameState(10, 10)
        state.position = (5, 5)
        state.move_in_direction(EAST)
        self.assertEqual(state.position, (6, 5))

    def test_wraps_around_north_edge(self):
        state = GameState(10, 10)
        state.position = (5, 0)
        state.move_in_direction(NORTH)
        self.assertEqual(state.position, (5, 9))

    def test_wraps_around_west_edge(self):
        state = GameState(10, 10)
        state.position = (0, 5)
        state.move_in_direction(WEST)
        self.assertEqual(state.position, (9, 5))


class TestApplyMove(unittest.TestCase):
    def test_moves_in_the_current_facing_direction(self):
        state = GameState(10, 10)
        state.position = (5, 5)
        state.direction = SOUTH
        state.apply_move()
        self.assertEqual(state.position, (5, 6))


class TestApplyEject(unittest.TestCase):
    def test_k_five_pushes_forward(self):
        # k=5 -> pusher faced the same way we do -> pushed straight ahead
        state = GameState(10, 10)
        state.position = (5, 5)
        state.direction = NORTH
        state.apply_eject(5)
        self.assertEqual(state.position, (5, 4))

    def test_k_three_pushes_right_of_facing_direction(self):
        state = GameState(10, 10)
        state.position = (5, 5)
        state.direction = NORTH
        state.apply_eject(3) # relative=1 -> facing+1 -> EAST
        self.assertEqual(state.position, (6, 5))

    def test_k_one_pushes_behind(self):
        state = GameState(10, 10)
        state.position = (5, 5)
        state.direction = NORTH
        state.apply_eject(1) # relative=2 -> opposite of facing -> SOUTH
        self.assertEqual(state.position, (5, 6))

    def test_k_seven_pushes_left_of_facing_direction(self):
        state = GameState(10, 10)
        state.position = (5, 5)
        state.direction = NORTH
        state.apply_eject(7) # relative=3 -> facing-1 -> WEST
        self.assertEqual(state.position, (4, 5))


class TestCurrentTile(unittest.TestCase):
    def test_none_without_vision(self):
        state = GameState(10, 10)
        self.assertIsNone(state.current_tile())

    def test_first_vision_entry_is_the_current_tile(self):
        state = GameState(10, 10)
        own_tile = Tile()
        state.update_vision([own_tile, Tile()])
        self.assertIs(state.current_tile(), own_tile)


if __name__ == "__main__":
    unittest.main()
