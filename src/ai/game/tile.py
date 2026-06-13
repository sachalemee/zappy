##
## EPITECH PROJECT, 2026
## zappy
## File description:
## tile
##


class Tile:
    RESOURCES = ("food",
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
        ## entry is one space-separated chunk of a `look` response,
        ## e.g. "player deraumere" or "" (empty tile) or "thystame"
        pass
