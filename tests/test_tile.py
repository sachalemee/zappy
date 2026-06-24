##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_tile
##

import unittest

from src.ai.game.tile import Tile


class TestFromLookEntry(unittest.TestCase):
    def test_empty_entry_is_an_empty_tile(self):
        tile = Tile.from_look_entry("")
        self.assertEqual(tile.players_here, 0)
        self.assertTrue(all(amount == 0 for amount in tile.resources.values()))

    def test_counts_a_single_player(self):
        tile = Tile.from_look_entry("player")
        self.assertEqual(tile.players_here, 1)

    def test_counts_multiple_players_on_the_same_tile(self):
        tile = Tile.from_look_entry("player player")
        self.assertEqual(tile.players_here, 2)

    def test_counts_resources(self):
        tile = Tile.from_look_entry("food linemate linemate")
        self.assertEqual(tile.resources["food"], 1)
        self.assertEqual(tile.resources["linemate"], 2)

    def test_mixed_players_and_resources(self):
        tile = Tile.from_look_entry("player deraumere phiras")
        self.assertEqual(tile.players_here, 1)
        self.assertEqual(tile.resources["deraumere"], 1)
        self.assertEqual(tile.resources["phiras"], 1)

    def test_ignores_unknown_words(self):
        tile = Tile.from_look_entry("egg")
        self.assertEqual(tile.players_here, 0)
        self.assertTrue(all(amount == 0 for amount in tile.resources.values()))


if __name__ == "__main__":
    unittest.main()
