##
## EPITECH PROJECT, 2026
## zappy
## File description:
## fsm
##

from enum import Enum, auto

from src.ai.algo.requirements import missing_resources, has_enough_players

SURVIVAL_THRESHOLD = 50 # food units below which survival takes priority over EVERYTHING else

FORK_FOOD_THRESHOLD = 100 # minimum food surplus needed before laying an egg (use of forkl)

FORK_RESOURCES = ( # pretty obvious lol
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

# Finite State Machine
class FSM:
    def __init__(self):
        self.state = State.COLLECT

    def ready_to_incantate(self, game_state) -> bool:
        missing = missing_resources(game_state.level, game_state.inventory.items)
        for amount in missing.values():
            if amount != 0:
                return False
        return True

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
