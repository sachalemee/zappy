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
        # inventory works like this : "[food 345, sibur 3, ...]" -> name/value pairs
        inventory = Inventory()
        content = line.strip().strip("[]")
        for chunk in content.split(","):
            parts = chunk.strip().split()
            if len(parts) != 2 or not parts[1].lstrip("-").isdigit():
                continue # ignore anything malformed instead of crashing
            inventory.items[parts[0]] = int(parts[1])
        return inventory

    def quantity_of(self, resource: str) -> int:
        return self.items.get(resource, 0)
