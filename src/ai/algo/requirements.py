##
## EPITECH PROJECT, 2026
## zappy
## File description:
## requirements
##

# elevation works like this : key = current level, values = players and stones needed on the ground to reach key+1 (subject's elevation table)
INCANTATION_REQUIREMENTS = {
    1: {
            "players": 1, "linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0
        },
    2: {
            "players": 2, "linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0
        },
    3: {
            "players": 2, "linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0
        },
    4: {
            "players": 4, "linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0
        },
    5: {
            "players": 4, "linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0
        },
    6: {
            "players": 6, "linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0
        },
    7: {
            "players": 6, "linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1
        }
}

def missing_resources(level: int, inventory: dict) -> dict:
    requirements = INCANTATION_REQUIREMENTS.get(level, {})
    missing = {}
    for resource, required_amount in requirements.items():
        if resource == "players":
            continue
        available_amount = inventory.get(resource, 0)
        missing[resource] = max(0, required_amount - available_amount)
    return missing

def has_enough_players(level: int, players_here: int) -> bool:
    required_players = INCANTATION_REQUIREMENTS.get(level, {}).get("players", 0)
    return players_here >= required_players
