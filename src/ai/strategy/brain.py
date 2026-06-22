##
## EPITECH PROJECT, 2026
## zappy
## File description:
## brain
##

from src.ai.game.state import GameState
from src.ai.game.tile import Tile
from src.ai.game.inventory import Inventory

from src.ai.strategy.actions import Action

from src.ai.algo.fsm import FSM, State
from src.ai.algo.requirements import INCANTATION_REQUIREMENTS, missing_resources
from src.ai.algo.navigator import Navigator
from src.ai.algo.coordinator import Coordinator

BROADCAST_COOLDOWN = 5 # ticks between two incantation call broadcasts
INVENTORY_POLL_INTERVAL = 20 # ticks between two Inventory refreshes
LOOK_POLL_INTERVAL = 5 # ticks between two Look refreshes while stationary

class Brain:
    def __init__(self, state: GameState):
        self.state = state
        self.fsm = FSM()
        self.navigator = Navigator()
        self.next_argument: str | None = None # argument for the next command (Take/Set/Broadcast)
        self._incantation_pending = False
        self._broadcast_cooldown = 0
        self._ticks_since_inventory = INVENTORY_POLL_INTERVAL
        self._ticks_since_look = LOOK_POLL_INTERVAL

    def decide(self) -> Action:
        self.next_argument = None

        self._ticks_since_look += 1
        self._ticks_since_inventory += 1

        if self._ticks_since_look >= LOOK_POLL_INTERVAL:
            self._ticks_since_look = 0
            return Action.LOOK

        if self._ticks_since_inventory >= INVENTORY_POLL_INTERVAL:
            self._ticks_since_inventory = 0
            return Action.INVENTORY

        self.fsm.transition(self.state)

        tile = self.state.current_tile()
        # eject works like this : pushes everyone else off the tile (and destroys their eggs there); only outside of GATHER/INCANTATE/BROADCAST so we never eject players we actually want to elevate with
        if self.fsm.state in (State.COLLECT, State.SURVIVE) and tile and tile.players_here > 1:
            return Action.EJECT

        if self.fsm.state == State.INCANTATE:
            # incantation works like this : frozen 300/f ticks, two replies ("Elevation underway" then "Current level: N"/"ko"); don't resend
            if self._incantation_pending:
                return Action.LOOK
            self._incantation_pending = True
            return Action.INCANTATION
        if self.fsm.state == State.FORK:
            return Action.FORK # lays an egg, adds one free slot to the team
        if self.fsm.state == State.BROADCAST:
            if self._broadcast_cooldown > 0:
                self._broadcast_cooldown -= 1
            else:
                self._broadcast_cooldown = BROADCAST_COOLDOWN
                self.next_argument = Coordinator.encode_incant_call(self.state.team_name, self.state.level)
                return Action.BROADCAST
        if self.fsm.state == State.GATHER:
            _, k = self.state.incoming_call
            if k == 0:
                self.state.incoming_call = None
            else:
                return Navigator.move_toward_sound(k)

        if self.fsm.state == State.SURVIVE:
            if tile and tile.resources.get("food", 0) > 0:
                self.next_argument = "food"
                return Action.TAKE
            return self.navigator.move_toward_ressource(self.state)

        if tile:
            # incantation needs stones on the ground, not in the bag
            tile_missing = missing_resources(self.state.level, tile.resources)
            for resource, amount in tile_missing.items():
                if amount > 0 and self.state.inventory.quantity_of(resource) > 0:
                    self.next_argument = resource
                    return Action.SET

            # only take what's in excess of this tile's own incantation requirement,
            # otherwise we'd take/set the same stone back and forth forever
            required_here = INCANTATION_REQUIREMENTS.get(self.state.level, {})
            for resource, quantity in tile.resources.items():
                if resource != "food" and quantity > required_here.get(resource, 0):
                    self.next_argument = resource
                    return Action.TAKE

        return self.navigator.move_toward_ressource(self.state)

    def handle_broadcast(self, line: str) -> bool:
        parsed = Coordinator.parse_broadcast_line(line)
        if parsed is None:
            return False

        k, text = parsed
        level = Coordinator.parse_incant_call(self.state.team_name, text)
        if level is not None and level == self.state.level:
            self.state.incoming_call = (level, k)
        return True

    def handle_eject(self, line: str) -> bool:
        line = line.strip()
        if not line.startswith("eject:"):
            return False
        try:
            k = int(line.split(":", 1)[1].strip())
        except ValueError:
            return False
        self.state.apply_eject(k)
        return True

    def handle_level_up(self, line: str) -> bool:
        line = line.strip()
        if not line.startswith("Current level:"):
            return False
        try:
            self.state.level = int(line.rsplit(":", 1)[-1].strip())
        except ValueError:
            return False
        return True

    def update(self, action: Action, response: str) -> None:
        response = response.strip()

        if action == Action.LOOK and response.startswith("["):
            # the tile separator is a comma, with or without a space after it
            entries = response.strip("[]").split(",")
            self.state.update_vision([Tile.from_look_entry(entry) for entry in entries])
        elif action == Action.INVENTORY and response.startswith("["):
            self.state.update_inventory(Inventory.from_response(response))
        elif action == Action.FORWARD and response == "ok":
            self.state.apply_move()
            self._ticks_since_look = LOOK_POLL_INTERVAL # tile changed, look again next tick
        elif action in (Action.LEFT, Action.RIGHT) and response == "ok":
            self.state.apply_turn(action)
        elif action == Action.TAKE and response == "ok" and self.next_argument:
            self.move_resource(self.next_argument, from_tile=True)
        elif action == Action.SET and response == "ok" and self.next_argument:
            self.move_resource(self.next_argument, from_tile=False)
        elif action == Action.INCANTATION:
            self._incantation_pending = False
            if "Current level" in response:
                self.state.level = int(response.rsplit(":", 1)[-1].strip())
        elif action == Action.FORK and response == "ok":
            # fork works like this : lays an egg and frees a team slot; we track it locally instead of polling Connect_nbr
            self.state.pending_spawns += 1
        elif action == Action.EJECT and response == "ok":
            self._ticks_since_look = LOOK_POLL_INTERVAL # tile just got cleared, refresh vision

    def move_resource(self, resource: str, from_tile: bool) -> None:
        tile = self.state.current_tile()
        inventory_amount = self.state.inventory.quantity_of(resource)

        if from_tile:
            self.state.inventory.items[resource] = inventory_amount + 1
            if tile:
                tile.resources[resource] = max(0, tile.resources.get(resource, 0) - 1)
        else:
            self.state.inventory.items[resource] = max(0, inventory_amount - 1)
            if tile:
                tile.resources[resource] = tile.resources.get(resource, 0) + 1
