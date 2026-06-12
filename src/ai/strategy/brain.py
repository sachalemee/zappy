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
from src.ai.algo.requirements import missing_resources
from src.ai.algo.navigator import Navigator

class Brain:
    def __init__(self, state: GameState):
        self.state = state
        self.fsm = FSM()
        self.navigator = Navigator()
        self.next_argument: str | None = None # argument to send alongside the last decided action (ex : resource for take/set)

    def decide(self) -> Action:
        self.next_argument = None
        self.fsm.transition(self.state)

        if self.fsm.state == State.INCANTATE:
            return Action.INCANTATION
        if self.fsm.state == State.FORK:
            return Action.FORK

        tile = self.state.current_tile()

        if self.fsm.state == State.SURVIVE and tile and tile.resources.get("food", 0) > 0:
            self.next_argument = "food"
            return Action.TAKE

        if tile:
            missing = missing_resources(self.state.level, self.state.inventory.items)
            for resource, amount in missing.items():
                if amount > 0 and tile.resources.get(resource, 0) > 0:
                    self.next_argument = resource
                    return Action.TAKE

        return self.navigator.move_toward_ressource(self.state)

    def update(self, action: Action, response: str) -> None:
        response = response.strip()

        if action == Action.LOOK and response.startswith("["):
            entries = response.strip("[]").split(",")
            self.state.update_vision([Tile.from_look_entry(entry) for entry in entries])
        elif action == Action.INVENTORY and response.startswith("["):
            self.state.update_inventory(Inventory.from_response(response))
        elif action == Action.FORWARD and response == "ok":
            self.state.apply_move()
        elif action in (Action.LEFT, Action.RIGHT) and response == "ok":
            self.state.apply_turn(action)
        elif action == Action.TAKE and response == "ok" and self.next_argument:
            self.move_resource(self.next_argument, from_tile=True)
        elif action == Action.SET and response == "ok" and self.next_argument:
            self.move_resource(self.next_argument, from_tile=False)
        elif action == Action.INCANTATION and "Current level" in response:
            self.state.level = int(response.rsplit(":", 1)[-1].strip())

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
