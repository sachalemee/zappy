##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_fsm
##

import unittest

from src.ai.algo.fsm import FSM, State, SURVIVAL_THRESHOLD, FORK_FOOD_THRESHOLD, FORK_RESOURCES
from src.ai.game.state import GameState
from src.ai.game.tile import Tile


def make_state(level: int = 1, food: int = 9999) -> GameState:
    state = GameState(10, 10, "team1")
    state.level = level
    state.inventory.items["food"] = food
    state.vision = [Tile()]
    return state


class TestReadyToIncantate(unittest.TestCase):
    def test_false_when_level_has_no_requirements(self):
        state = make_state(level=8) # max level, not in the table
        self.assertFalse(FSM().ready_to_incantate(state))

    def test_false_when_no_vision(self):
        state = make_state(level=1)
        state.vision = []
        self.assertFalse(FSM().ready_to_incantate(state))

    def test_false_when_ground_is_missing_a_stone(self):
        state = make_state(level=1) # needs 1 linemate on the ground
        self.assertFalse(FSM().ready_to_incantate(state))

    def test_true_when_ground_satisfies_the_requirement(self):
        state = make_state(level=1)
        state.current_tile().resources["linemate"] = 1
        self.assertTrue(FSM().ready_to_incantate(state))

    def test_true_with_surplus_on_the_ground(self):
        state = make_state(level=1)
        state.current_tile().resources["linemate"] = 5 # more than required is still satisfied
        self.assertTrue(FSM().ready_to_incantate(state))


class TestHasForkSurplus(unittest.TestCase):
    def test_false_when_food_below_threshold(self):
        state = make_state(food=FORK_FOOD_THRESHOLD - 1)
        for resource in FORK_RESOURCES:
            state.inventory.items[resource] = 1
        self.assertFalse(FSM().has_fork_surplus(state))

    def test_false_when_missing_one_stone(self):
        state = make_state(food=FORK_FOOD_THRESHOLD)
        for resource in FORK_RESOURCES[:-1]:
            state.inventory.items[resource] = 1
        # the last FORK_RESOURCES entry is left at 0
        self.assertFalse(FSM().has_fork_surplus(state))

    def test_true_when_everything_is_there(self):
        state = make_state(food=FORK_FOOD_THRESHOLD)
        for resource in FORK_RESOURCES:
            state.inventory.items[resource] = 1
        self.assertTrue(FSM().has_fork_surplus(state))


class TestTransitionPriority(unittest.TestCase):
    def test_starving_always_wins(self):
        state = make_state(food=SURVIVAL_THRESHOLD - 1)
        # even with everything else ready, starving takes priority
        state.current_tile().resources["linemate"] = 1
        state.current_tile().players_here = 1
        fsm = FSM()
        fsm.transition(state)
        self.assertEqual(fsm.state, State.SURVIVE)

    def test_incantate_when_resources_and_players_ready(self):
        state = make_state(level=1)
        state.current_tile().resources["linemate"] = 1
        state.current_tile().players_here = 1 # level 1 only needs 1 player
        fsm = FSM()
        fsm.transition(state)
        self.assertEqual(fsm.state, State.INCANTATE)

    def test_gather_takes_priority_over_broadcast_when_someone_else_calls(self):
        # regression: a player ready to broadcast for its own spot must instead join
        # a teammate's call, or two ready players would wait on each other forever
        state = make_state(level=2)
        state.current_tile().resources.update({"linemate": 1, "deraumere": 1, "sibur": 1})
        state.current_tile().players_here = 1 # alone, not enough for level 2 (needs 2)
        state.incoming_call = (2, 3) # a teammate is calling for the same level
        fsm = FSM()
        fsm.transition(state)
        self.assertEqual(fsm.state, State.GATHER)

    def test_gather_ignored_when_call_is_for_a_different_level(self):
        state = make_state(level=2)
        state.incoming_call = (5, 3) # not our level
        fsm = FSM()
        fsm.transition(state)
        self.assertNotEqual(fsm.state, State.GATHER)

    def test_fork_takes_priority_over_broadcast_when_alone(self):
        # regression: a lone player with a fork-worthy stockpile used to broadcast into
        # the void forever instead of growing its own team
        state = make_state(level=2, food=FORK_FOOD_THRESHOLD)
        for resource in FORK_RESOURCES:
            state.inventory.items[resource] = 1
        state.current_tile().resources.update({"linemate": 1, "deraumere": 1, "sibur": 1})
        fsm = FSM()
        fsm.transition(state)
        self.assertEqual(fsm.state, State.FORK)

    def test_broadcast_when_resources_ready_but_no_fork_surplus_and_alone(self):
        state = make_state(level=2)
        state.current_tile().resources.update({"linemate": 1, "deraumere": 1, "sibur": 1})
        fsm = FSM()
        fsm.transition(state)
        self.assertEqual(fsm.state, State.BROADCAST)

    def test_collect_is_the_default(self):
        state = make_state(level=1) # nothing on the ground, no call, no fork surplus
        fsm = FSM()
        fsm.transition(state)
        self.assertEqual(fsm.state, State.COLLECT)


if __name__ == "__main__":
    unittest.main()
