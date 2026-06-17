##
## EPITECH PROJECT, 2026
## zappy
## File description:
## tile
##


class Tile:
    RESOURCES = (
        "food",
        "linemate",
        "deraumere",
        "sibur",
        "mendiane",
        "phiras",
        "thystame"
    )

    def __init__(self):
        self.resources: dict[str, int] = {name: 0 for name in Tile.RESOURCES}
        self.players_here = 0

    @staticmethod
    def from_look_entry(entry: str) -> "Tile":
        tile = Tile()
        for word in entry.split():
            if word == "player":
                tile.players_here += 1
            elif word in tile.resources:
                tile.resources[word] += 1
        return tile
