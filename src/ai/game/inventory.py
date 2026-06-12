##
## EPITECH PROJECT, 2026
## zappy
## File description:
## inventory
##

from src.ai.game.tile import Tile


class Inventory:
    def __init__(self):
        self.items: dict[str, int] = {name: 0 for name in Tile.RESOURCES}

    @staticmethod
    def from_response(line: str) -> "Inventory":
        ## line looks like "[food 345, sibur 3, phiras 5, ...]"
        inventory = Inventory()
        content = line.strip("[]")
        for chunk in content.split(","):
            name, _, quantity = chunk.strip().partition(" ")
            if name in inventory.items:
                inventory.items[name] = int(quantity)
        return inventory

    def quantity_of(self, resource: str) -> int:
        return self.items.get(resource, 0)
