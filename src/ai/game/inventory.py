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
        inventory = Inventory()
        content = line.strip().strip("[]")
        for chunk in content.split(","):
            chunk = chunk.strip()
            if not chunk:
                continue
            name, value = chunk.split()
            inventory.items[name] = int(value)
        return inventory

    def quantity_of(self, resource: str) -> int:
        return self.items.get(resource, 0)
