##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_navigator
##

import unittest

from src.ai.algo.navigator import Navigator
from src.ai.strategy.actions import Action


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


class TestMoveTowardSound(unittest.TestCase):
    def test_k_one_is_straight_ahead(self):
        self.assertEqual(Navigator.move_toward_sound(1), Action.FORWARD)

    def test_left_octants(self):
        # per the subject's "trigonomic" (counter-clockwise) diagram, 2/3/4 are on our left
        for k in (2, 3, 4):
            self.assertEqual(Navigator.move_toward_sound(k), Action.LEFT, msg=f"k={k}")

    def test_right_octants(self):
        # 5/6/7/8 are on our right (5 = straight behind, arbitrarily resolved to the right)
        for k in (5, 6, 7, 8):
            self.assertEqual(Navigator.move_toward_sound(k), Action.RIGHT, msg=f"k={k}")


if __name__ == "__main__":
    unittest.main()
