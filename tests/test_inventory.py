##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_inventory
##

import unittest

from src.ai.game.inventory import Inventory


class TestInventory(unittest.TestCase):
    def test_starts_empty(self):
        inventory = Inventory()
        self.assertEqual(inventory.quantity_of("food"), 0)

    def test_quantity_of_unknown_resource_is_zero(self):
        inventory = Inventory()
        self.assertEqual(inventory.quantity_of("unknown"), 0)


class TestFromResponse(unittest.TestCase):
    def test_parses_name_value_pairs(self):
        inventory = Inventory.from_response("[food 345, sibur 3]")
        self.assertEqual(inventory.quantity_of("food"), 345)
        self.assertEqual(inventory.quantity_of("sibur"), 3)

    def test_unmentioned_resources_stay_at_zero(self):
        inventory = Inventory.from_response("[food 345]")
        self.assertEqual(inventory.quantity_of("linemate"), 0)

    def test_ignores_malformed_chunks(self):
        inventory = Inventory.from_response("[food 345, garbage, sibur abc]")
        self.assertEqual(inventory.quantity_of("food"), 345)
        self.assertEqual(inventory.quantity_of("sibur"), 0)

    def test_strips_brackets_and_whitespace(self):
        inventory = Inventory.from_response("[ food 345 ]")
        self.assertEqual(inventory.quantity_of("food"), 345)


if __name__ == "__main__":
    unittest.main()
