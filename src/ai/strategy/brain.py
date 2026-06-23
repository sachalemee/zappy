##
## EPITECH PROJECT, 2026
## zappy
## File description:
## brain
##

import os

from src.ai.game.state import GameState
from src.ai.game.tile import Tile
from src.ai.game.inventory import Inventory

from src.ai.strategy.actions import Action

from src.ai.algo.fsm import FSM, State, FORK_FOOD_THRESHOLD
from src.ai.algo.requirements import INCANTATION_REQUIREMENTS, missing_resources
from src.ai.algo.navigator import Navigator
from src.ai.algo.coordinator import Coordinator

BROADCAST_COOLDOWN = 5 # ticks between two incantation call broadcasts
BROADCAST_GIVEUP_LIMIT = 5 # consecutive unanswered calls before abandoning this spot
INVENTORY_POLL_INTERVAL = 20 # ticks between two Inventory refreshes
LOOK_POLL_INTERVAL = 5 # ticks between two Look refreshes while stationary
CONNECT_NBR_POLL_INTERVAL = 20 # ticks between two Connect_nbr refreshes

class Brain:
    def __init__(self, state: GameState):
        self.state = state
        self.fsm = FSM()
        # each AI process gets a different exploration order so siblings spawned
        # near each other (e.g. via Fork) fan out instead of retracing the same path
        self.navigator = Navigator(start_offset=os.getpid() % 4)
        self.next_argument: str | None = None # argument for the next command (Take/Set/Broadcast)
        self._incantation_pending = False
        self._depositing = False # once the bag has everything needed, stay in deposit mode until the ground does too (avoids take/set flip-flopping on the same stone)
        self._last_gather_k = None # last broadcast direction we already turned for, so a stale k advances instead of re-turning
        self._broadcast_cooldown = 0
        self._broadcast_attempts = 0 # consecutive Broadcasts sent with nobody showing up
        self._just_gave_up = False # reclaimed our stones after giving up ; walk away before trying this spot again
        self._reported_resources: dict[str, tuple[int, int]] = {} # resource -> position we last broadcast it from, so we don't repeat every tick
        self._ticks_since_inventory = INVENTORY_POLL_INTERVAL
        self._ticks_since_look = LOOK_POLL_INTERVAL
        self._ticks_since_connect_nbr = CONNECT_NBR_POLL_INTERVAL

    def decide(self) -> Action:
        self.next_argument = None

        self._ticks_since_look += 1
        self._ticks_since_inventory += 1
        self._ticks_since_connect_nbr += 1

        if self._ticks_since_look >= LOOK_POLL_INTERVAL:
            self._ticks_since_look = 0
            return Action.LOOK

        if self._ticks_since_inventory >= INVENTORY_POLL_INTERVAL:
            self._ticks_since_inventory = 0
            return Action.INVENTORY

        if self._ticks_since_connect_nbr >= CONNECT_NBR_POLL_INTERVAL:
            self._ticks_since_connect_nbr = 0
            return Action.CONNECT_NBR

        self.fsm.transition(self.state)

        if self.fsm.state != State.BROADCAST:
            self._broadcast_attempts = 0 # left the state (joined someone, leveled up...) ; start fresh next time

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
            if self._broadcast_attempts >= BROADCAST_GIVEUP_LIMIT:
                # nobody's coming ; give up on this spot and fall through to the gather
                # logic below, which will pick our own dropped stones back up since
                # they're sitting right here and our bag is short of them again
                self._broadcast_attempts = 0
                self._just_gave_up = True
            elif self._broadcast_cooldown > 0:
                self._broadcast_cooldown -= 1
                return Action.LOOK # stay put and wait for the cooldown ; falling through would
                                   # re-take the stones we just deposited here for the call
            else:
                self._broadcast_cooldown = BROADCAST_COOLDOWN
                self._broadcast_attempts += 1
                self.next_argument = Coordinator.encode_incant_call(self.state.team_name, self.state.level)
                return Action.BROADCAST
        if self.fsm.state == State.GATHER:
            _, k = self.state.incoming_call
            if k == 0:
                self.state.incoming_call = None
                self._last_gather_k = None
            elif k == self._last_gather_k:
                # no fresh broadcast since we turned for this k ; advance instead of
                # re-issuing the same turn every tick (that would just spin in place)
                return Action.FORWARD
            else:
                self._last_gather_k = k
                return Navigator.move_toward_sound(k)

        if self.fsm.state == State.SURVIVE:
            if tile and tile.resources.get("food", 0) > 0:
                self.next_argument = "food"
                return Action.TAKE
            return self.navigator.move_toward_ressource(self.state)

        if tile:
            for resource, quantity in tile.resources.items():
                if resource == "food" or quantity <= 0:
                    continue
                if self._reported_resources.get(resource) == self.state.position:
                    continue
                self._reported_resources[resource] = self.state.position
                self.next_argument = Coordinator.encode_resource_report(
                    self.state.team_name, resource, *self.state.position)
                return Action.BROADCAST

            bag_missing = missing_resources(self.state.level, self.state.inventory.items)
            bag_complete = not any(amount > 0 for amount in bag_missing.values())

            if bag_complete and self._just_gave_up:
                # just reclaimed everything we'd dropped here ; move on before retrying,
                # or we'd immediately deposit the same stones on this same spot again
                self._just_gave_up = False
                return self.navigator.move_toward_ressource(self.state)

            if not self._depositing and bag_complete:
                self._depositing = True # bag is fully stocked for this level, commit to dropping it here

            if self._depositing:
                # incantation needs stones on the ground, not in the bag ; deposit everything
                # we're carrying before ever taking anything again, so we don't undo our own drop
                tile_missing = missing_resources(self.state.level, tile.resources)
                for resource, amount in tile_missing.items():
                    if amount > 0 and self.state.inventory.quantity_of(resource) > 0:
                        self.next_argument = resource
                        return Action.SET
                self._depositing = False # nothing left to drop here, free to gather again
            else:
                # gather up to what incantation needs (at least 1 of each, even past the
                # requirement, so a fork-worthy stockpile can build up too) ; safe from
                # flip-flopping with Set since we only get here while _depositing is False ;
                # food is topped up the same way instead of only when already starving,
                # otherwise it never climbs high enough to afford forking
                required_here = INCANTATION_REQUIREMENTS.get(self.state.level, {})
                for resource, quantity in tile.resources.items():
                    if resource == "food":
                        target = FORK_FOOD_THRESHOLD
                    else:
                        target = max(required_here.get(resource, 0), 1)
                    if quantity > 0 and self.state.inventory.quantity_of(resource) < target:
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

        report = Coordinator.parse_resource_report(self.state.team_name, text)
        if report is not None:
            resource, x, y = report
            self.state.known_resources[resource] = (x, y)
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
            # fork works like this : lays an egg and frees a team slot ; the real count (which also moves from other processes' forks or
            # eggs already on the board at startup) only comes from the server, so re-poll Connect_nbr next tick instead of guessing
            self._ticks_since_connect_nbr = CONNECT_NBR_POLL_INTERVAL
        elif action == Action.CONNECT_NBR:
            try:
                self.state.known_free_slots = int(response)
            except ValueError:
                pass
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
