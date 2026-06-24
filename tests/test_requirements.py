##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_requirements
##

import unittest

from src.ai.algo.requirements import missing_resources, has_enough_players


class TestMissingResources(unittest.TestCase):
    def test_everything_missing_from_an_empty_inventory(self):
        missing = missing_resources(2, {})
        self.assertEqual(missing, {
            "linemate": 1, "deraumere": 1, "sibur": 1,
            "mendiane": 0, "phiras": 0, "thystame": 0,
        })

    def test_nothing_missing_once_satisfied(self):
        missing = missing_resources(1, {"linemate": 1})
        self.assertEqual(missing["linemate"], 0)

    def test_never_negative_with_a_surplus(self):
        missing = missing_resources(1, {"linemate": 5})
        self.assertEqual(missing["linemate"], 0)

    def test_does_not_report_the_players_key(self):
        missing = missing_resources(2, {})
        self.assertNotIn("players", missing)

    def test_unknown_level_has_no_requirements(self):
        self.assertEqual(missing_resources(8, {}), {})


class TestHasEnoughPlayers(unittest.TestCase):
    def test_true_when_exact_amount(self):
        self.assertTrue(has_enough_players(2, 2))

    def test_true_with_surplus(self):
        self.assertTrue(has_enough_players(2, 5))

    def test_false_when_short(self):
        self.assertFalse(has_enough_players(2, 1))

    def test_unknown_level_requires_no_players(self):
        self.assertTrue(has_enough_players(8, 0))


if __name__ == "__main__":
    unittest.main()
