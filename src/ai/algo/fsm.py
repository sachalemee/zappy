##
## EPITECH PROJECT, 2026
## zappy
## File description:
## fsm
##

from enum import Enum, auto

from src.ai.algo.requirements import INCANTATION_REQUIREMENTS, missing_resources, has_enough_players

# food works like this : 1 unit = 126 time units of life, so this
# threshold gives a comfortable margin before actually starving
SURVIVAL_THRESHOLD = 50

FORK_FOOD_THRESHOLD = 100 # min food surplus before forking

FORK_RESOURCES = (
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame",
)

class State(Enum):
    SURVIVE = auto()
    COLLECT = auto()
    BROADCAST = auto()
    GATHER = auto()
    INCANTATE = auto()
    FORK = auto()

class FSM:
    def __init__(self):
        self.state = State.COLLECT

    def ready_to_incantate(self, game_state) -> bool:
        # incantation works like this : stones must be laid on the ground tile, not just carried in the bag (subject's elevation ritual)
        if game_state.level not in INCANTATION_REQUIREMENTS: # level 8 is max
            return False
        tile = game_state.current_tile()
        if tile is None:
            return False
        missing = missing_resources(game_state.level, tile.resources)
        return all(amount == 0 for amount in missing.values())

    def has_fork_surplus(self, game_state) -> bool:
        if game_state.inventory.quantity_of("food") < FORK_FOOD_THRESHOLD:
            return False
        for resource in FORK_RESOURCES:
            if game_state.inventory.quantity_of(resource) < 1:
                return False
        return True

    def transition(self, game_state) -> None:
        # priority order : survive first, then elevate, then grow, then explore
        if game_state.inventory.quantity_of("food") < SURVIVAL_THRESHOLD:
            self.state = State.SURVIVE
            return

        tile = game_state.current_tile()
        resources_ready = self.ready_to_incantate(game_state)
        players_ready = tile is not None and has_enough_players(game_state.level, tile.players_here)

        if resources_ready and players_ready:
            self.state = State.INCANTATE # everything is here, level up now
        elif resources_ready and not players_ready:
            self.state = State.BROADCAST # call for help
        elif game_state.incoming_call is not None and game_state.incoming_call[0] == game_state.level:
            self.state = State.GATHER # a teammate is calling
        elif self.has_fork_surplus(game_state):
            self.state = State.FORK # enough surplus, grow the team
        else:
            self.state = State.COLLECT # default : explore and gather
