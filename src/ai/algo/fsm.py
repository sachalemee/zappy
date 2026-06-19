##
## EPITECH PROJECT, 2026
## zappy
## File description:
## fsm
##

from enum import Enum, auto

from src.ai.algo.requirements import INCANTATION_REQUIREMENTS, missing_resources, has_enough_players

SURVIVAL_THRESHOLD = 50 # below this food count, survival takes priority

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
        if game_state.level not in INCANTATION_REQUIREMENTS: # already at max level
            return False
        tile = game_state.current_tile()
        if tile is None:
            return False
        missing = missing_resources(game_state.level, tile.resources) # stones must be on the ground
        return all(amount == 0 for amount in missing.values())

    def has_fork_surplus(self, game_state) -> bool:
        if game_state.inventory.quantity_of("food") < FORK_FOOD_THRESHOLD:
            return False
        for resource in FORK_RESOURCES:
            if game_state.inventory.quantity_of(resource) < 1:
                return False
        return True

    def transition(self, game_state) -> None:
        if game_state.inventory.quantity_of("food") < SURVIVAL_THRESHOLD:
            self.state = State.SURVIVE
            return

        tile = game_state.current_tile()
        resources_ready = self.ready_to_incantate(game_state)
        players_ready = tile is not None and has_enough_players(game_state.level, tile.players_here)

        if resources_ready and players_ready:
            self.state = State.INCANTATE
        elif resources_ready and not players_ready:
            self.state = State.BROADCAST
        elif game_state.incoming_call is not None and game_state.incoming_call[0] == game_state.level:
            self.state = State.GATHER
        elif self.has_fork_surplus(game_state):
            self.state = State.FORK
        else:
            self.state = State.COLLECT
