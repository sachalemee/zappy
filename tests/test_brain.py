##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_brain
##

import unittest

from src.ai.algo.coordinator import Coordinator
from src.ai.algo.navigator import Navigator
from src.ai.game.state import GameState
from src.ai.game.tile import Tile
from src.ai.strategy.actions import Action
from src.ai.strategy.brain import Brain


def make_brain(level: int = 1, food: int = 9999) -> Brain:
    state = GameState(10, 10, "team1")
    state.level = level
    state.inventory.items["food"] = food
    state.vision = [Tile()]
    brain = Brain(state)
    # pin a deterministic exploration order : Brain normally derives it from
    # os.getpid() % 4, which would make movement assertions flaky here
    brain.navigator = Navigator(start_offset=0)
    # bypass the periodic Look/Inventory/Connect_nbr polling so each test can
    # focus on the FSM-driven branch it's actually exercising
    brain._ticks_since_look = 0
    brain._ticks_since_inventory = 0
    brain._ticks_since_connect_nbr = 0
    return brain


class TestPeriodicPolling(unittest.TestCase):
    def test_forces_look_first(self):
        state = GameState(10, 10, "team1")
        brain = Brain(state) # ticks start already at their interval value
        self.assertEqual(brain.decide(), Action.LOOK)

    def test_forces_inventory_after_look_interval(self):
        brain = make_brain()
        brain._ticks_since_inventory = 999
        self.assertEqual(brain.decide(), Action.INVENTORY)

    def test_forces_connect_nbr_after_its_interval(self):
        brain = make_brain()
        brain._ticks_since_connect_nbr = 999
        self.assertEqual(brain.decide(), Action.CONNECT_NBR)


class TestEject(unittest.TestCase):
    def test_ejects_when_crowded_while_collecting(self):
        brain = make_brain()
        brain.state.current_tile().players_here = 2
        self.assertEqual(brain.decide(), Action.EJECT)

    def test_does_not_eject_when_alone(self):
        brain = make_brain()
        brain.state.current_tile().players_here = 1
        self.assertNotEqual(brain.decide(), Action.EJECT)

    def test_does_not_eject_while_gathering_toward_a_call(self):
        # regression: never eject teammates we actually want to elevate with
        brain = make_brain(level=2)
        brain.state.current_tile().players_here = 2
        brain.state.incoming_call = (2, 3)
        self.assertNotEqual(brain.decide(), Action.EJECT)


class TestIncantation(unittest.TestCase):
    def test_sends_incantation_when_ready(self):
        brain = make_brain(level=1)
        brain.state.current_tile().resources["linemate"] = 1
        brain.state.current_tile().players_here = 1
        self.assertEqual(brain.decide(), Action.INCANTATION)
        self.assertTrue(brain._incantation_pending)

    def test_waits_with_look_instead_of_resending(self):
        brain = make_brain(level=1)
        brain.state.current_tile().resources["linemate"] = 1
        brain.state.current_tile().players_here = 1
        brain.decide() # first call : sends Incantation
        brain._ticks_since_look = 0 # bypass the periodic poll again for this tick
        self.assertEqual(brain.decide(), Action.LOOK)

    def test_update_clears_pending_and_applies_new_level_on_success(self):
        brain = make_brain(level=1)
        brain._incantation_pending = True
        brain.update(Action.INCANTATION, "Current level: 2")
        self.assertFalse(brain._incantation_pending)
        self.assertEqual(brain.state.level, 2)

    def test_update_clears_pending_without_changing_level_on_ko(self):
        brain = make_brain(level=1)
        brain._incantation_pending = True
        brain.update(Action.INCANTATION, "ko")
        self.assertFalse(brain._incantation_pending)
        self.assertEqual(brain.state.level, 1)


class TestFork(unittest.TestCase):
    def test_sends_fork_when_surplus_ready(self):
        from src.ai.algo.fsm import FORK_FOOD_THRESHOLD, FORK_RESOURCES
        brain = make_brain(level=1, food=FORK_FOOD_THRESHOLD)
        for resource in FORK_RESOURCES:
            brain.state.inventory.items[resource] = 1
        self.assertEqual(brain.decide(), Action.FORK)

    def test_fork_response_reschedules_connect_nbr_poll(self):
        from src.ai.strategy.brain import CONNECT_NBR_POLL_INTERVAL
        brain = make_brain()
        brain._ticks_since_connect_nbr = 0
        brain.update(Action.FORK, "ok")
        self.assertEqual(brain._ticks_since_connect_nbr, CONNECT_NBR_POLL_INTERVAL)


class TestBroadcastGiveUp(unittest.TestCase):
    def _setup_broadcasting(self) -> Brain:
        brain = make_brain(level=2)
        tile = brain.state.current_tile()
        tile.resources.update({"linemate": 1, "deraumere": 1, "sibur": 1})
        tile.players_here = 1 # alone : level 2 needs 2
        return brain

    def test_broadcasts_the_incant_call(self):
        brain = self._setup_broadcasting()
        action = brain.decide()
        self.assertEqual(action, Action.BROADCAST)
        self.assertEqual(
            brain.next_argument,
            Coordinator.encode_incant_call(brain.state.team_name, brain.state.level, brain._caller_id),
        )

    def test_waits_out_the_cooldown_with_look(self):
        brain = self._setup_broadcasting()
        brain.decide() # first Broadcast, starts the cooldown
        brain._ticks_since_look = 0
        self.assertEqual(brain.decide(), Action.LOOK)

    def test_gives_up_after_the_attempt_limit_and_reclaims_its_stones(self):
        from src.ai.strategy.brain import BROADCAST_GIVEUP_LIMIT
        brain = self._setup_broadcasting()
        # level 2 needs 2 players, so the effective limit is scaled by that
        brain._broadcast_attempts = BROADCAST_GIVEUP_LIMIT * 2
        action = brain.decide()
        self.assertTrue(brain._just_gave_up)
        # falls through to the gather logic, which picks our own dropped stone back up
        self.assertEqual(action, Action.TAKE)
        self.assertEqual(brain._broadcast_attempts, 0)

    def test_does_not_give_up_early_when_more_players_are_required(self):
        from src.ai.strategy.brain import BROADCAST_GIVEUP_LIMIT
        brain = make_brain(level=4)
        tile = brain.state.current_tile()
        tile.resources.update({"linemate": 1, "deraumere": 1, "sibur": 2, "phiras": 1})
        tile.players_here = 1 # alone : level 4 needs 4
        brain._broadcast_attempts = BROADCAST_GIVEUP_LIMIT # would have given up for level 2
        self.assertEqual(brain.decide(), Action.BROADCAST)
        self.assertFalse(brain._just_gave_up)

    def test_resets_attempts_when_a_sibling_joins(self):
        from src.ai.strategy.brain import BROADCAST_GIVEUP_LIMIT
        brain = make_brain(level=4)
        tile = brain.state.current_tile()
        tile.resources.update({"linemate": 1, "deraumere": 1, "sibur": 2, "phiras": 1})
        tile.players_here = 1
        brain._broadcast_attempts = BROADCAST_GIVEUP_LIMIT * 4 - 1 # one tick away from giving up
        brain.decide() # records players_here=1 as the baseline
        tile.players_here = 2 # a sibling just arrived
        brain.decide()
        self.assertEqual(brain._broadcast_attempts, 0)
        self.assertFalse(brain._just_gave_up)


class TestGather(unittest.TestCase):
    def test_clears_the_call_on_arrival(self):
        brain = make_brain(level=2)
        brain.state.incoming_call = (2, 0) # k=0 : already on the emitter's tile
        brain.decide()
        self.assertIsNone(brain.state.incoming_call)
        self.assertIsNone(brain._last_gather_k)

    def test_turns_once_for_a_fresh_k(self):
        brain = make_brain(level=2)
        brain.state.incoming_call = (2, 3) # k=3 -> left, per move_toward_sound
        self.assertEqual(brain.decide(), Action.LEFT)
        self.assertEqual(brain._last_gather_k, 3)

    def test_advances_instead_of_re_turning_on_a_stale_k(self):
        brain = make_brain(level=2)
        brain.state.incoming_call = (2, 3)
        brain.decide() # turns once, remembers k=3
        self.assertEqual(brain.decide(), Action.FORWARD)


class TestSurvive(unittest.TestCase):
    def test_eats_food_on_the_current_tile(self):
        brain = make_brain(food=10) # below SURVIVAL_THRESHOLD
        brain.state.current_tile().resources["food"] = 3
        self.assertEqual(brain.decide(), Action.TAKE)
        self.assertEqual(brain.next_argument, "food")

    def test_moves_to_explore_when_no_food_here(self):
        brain = make_brain(food=10)
        self.assertEqual(brain.decide(), Action.FORWARD)


class TestGatherAndDepositResources(unittest.TestCase):
    def test_takes_a_single_needed_stone(self):
        # regression: a lone stone on the ground used to never be picked up because
        # it wasn't "in excess" of the incantation requirement
        brain = make_brain(level=2)
        brain.state.current_tile().resources["linemate"] = 1
        self.assertEqual(brain.decide(), Action.TAKE)
        self.assertEqual(brain.next_argument, "linemate")

    def test_does_not_take_food_only_up_to_fork_threshold(self):
        from src.ai.algo.fsm import FORK_FOOD_THRESHOLD
        brain = make_brain(level=2, food=FORK_FOOD_THRESHOLD)
        brain.state.current_tile().resources["food"] = 3
        # already at the fork threshold ; no need to take more food here
        self.assertNotEqual((brain.decide(), brain.next_argument), (Action.TAKE, "food"))

    def test_deposits_once_the_bag_is_complete_for_the_level(self):
        brain = make_brain(level=1)
        brain.state.inventory.items["linemate"] = 1 # everything level 1 needs
        self.assertEqual(brain.decide(), Action.SET)
        self.assertEqual(brain.next_argument, "linemate")
        self.assertTrue(brain._depositing)

    def test_does_not_re_take_what_it_just_deposited(self):
        # regression: depositing then immediately re-gathering the same stone forever
        brain = make_brain(level=1)
        brain.state.inventory.items["linemate"] = 1
        brain.decide() # Set
        brain.update(Action.SET, "ok") # ground now has it, bag is now empty
        action = brain.decide()
        self.assertNotEqual(action, Action.TAKE)

    def test_moves_away_after_reclaiming_instead_of_redepositing_here(self):
        # regression: giving up then refilling the bag used to redeposit on the exact
        # same spot, recreating the same stuck broadcast loop
        brain = make_brain(level=1)
        brain._just_gave_up = True
        brain.state.inventory.items["linemate"] = 1 # bag already complete again
        action = brain.decide()
        self.assertEqual(action, Action.FORWARD)
        self.assertFalse(brain._just_gave_up)


class TestUpdateMovement(unittest.TestCase):
    def test_forward_applies_the_move_and_forces_a_look(self):
        brain = make_brain()
        brain._ticks_since_look = 0
        brain.update(Action.FORWARD, "ok")
        self.assertEqual(brain.state.position, (0, 9)) # facing north by default
        self.assertGreaterEqual(brain._ticks_since_look, 5)

    def test_forward_ko_does_not_move(self):
        brain = make_brain()
        brain.update(Action.FORWARD, "ko")
        self.assertEqual(brain.state.position, (0, 0))

    def test_right_turns_the_direction(self):
        brain = make_brain()
        brain.update(Action.RIGHT, "ok")
        self.assertEqual(brain.state.direction, 1) # NORTH -> EAST


class TestUpdateLookAndInventory(unittest.TestCase):
    def test_look_updates_vision(self):
        brain = make_brain()
        brain.update(Action.LOOK, "[player,,food]")
        self.assertEqual(len(brain.state.vision), 3)
        self.assertEqual(brain.state.vision[0].players_here, 1)
        self.assertEqual(brain.state.vision[2].resources["food"], 1)

    def test_inventory_updates_the_bag(self):
        brain = make_brain()
        brain.update(Action.INVENTORY, "[food 12, linemate 3]")
        self.assertEqual(brain.state.inventory.quantity_of("food"), 12)
        self.assertEqual(brain.state.inventory.quantity_of("linemate"), 3)

    def test_connect_nbr_updates_known_free_slots(self):
        brain = make_brain()
        brain.update(Action.CONNECT_NBR, "3")
        self.assertEqual(brain.state.known_free_slots, 3)


class TestHandlePushedLines(unittest.TestCase):
    def test_handle_broadcast_sets_incoming_call_for_matching_level(self):
        brain = make_brain(level=2)
        line = f"message 5, {Coordinator.encode_incant_call('team1', 2, 999)}"
        self.assertTrue(brain.handle_broadcast(line))
        self.assertEqual(brain.state.incoming_call, (2, 5))
        self.assertEqual(brain._following_caller_id, 999)

    def test_handle_broadcast_consumes_but_ignores_unrelated_messages(self):
        # any "message ..." line is recognized as a broadcast (so main.py won't
        # treat it as an unhandled push), but only our own protocol sets incoming_call
        brain = make_brain(level=2)
        self.assertTrue(brain.handle_broadcast("message 5, hello there"))
        self.assertIsNone(brain.state.incoming_call)

    def test_handle_broadcast_rejects_non_message_lines(self):
        brain = make_brain(level=2)
        self.assertFalse(brain.handle_broadcast("dead"))

    def test_handle_broadcast_stores_resource_report(self):
        brain = make_brain(level=1)
        line = f"message 6, {Coordinator.encode_resource_report('team1', 'sibur', 2, 9)}"
        self.assertTrue(brain.handle_broadcast(line))
        self.assertEqual(brain.state.known_resources["sibur"], (2, 9))

    def test_handle_eject_moves_the_player(self):
        brain = make_brain()
        self.assertTrue(brain.handle_eject("eject: 5")) # k=5 -> pusher faced the same way -> pushed forward
        self.assertEqual(brain.state.position, (0, 9)) # pushed forward (north)

    def test_handle_level_up_sets_the_new_level(self):
        brain = make_brain(level=3)
        self.assertTrue(brain.handle_level_up("Current level: 4"))
        self.assertEqual(brain.state.level, 4)


class TestResourceSharing(unittest.TestCase):
    def test_broadcasts_a_newly_seen_stone(self):
        # deraumere isn't needed at level 1 and we already hold the one we'd ever
        # want, so gathering won't claim it itself ; the surplus on the ground gets
        # reported to siblings instead
        brain = make_brain(level=1)
        brain.state.position = (3, 4)
        brain.state.inventory.items["deraumere"] = 1
        brain.state.current_tile().resources["deraumere"] = 2
        action = brain.decide()
        self.assertEqual(action, Action.BROADCAST)
        self.assertEqual(
            brain.next_argument,
            Coordinator.encode_resource_report("team1", "deraumere", 3, 4),
        )
        self.assertEqual(brain._reported_resources["deraumere"], (3, 4))

    def test_does_not_re_broadcast_the_same_spot(self):
        brain = make_brain(level=1)
        brain.state.inventory.items["deraumere"] = 1
        brain.state.current_tile().resources["deraumere"] = 2
        brain.decide() # first call : broadcasts the discovery
        brain._ticks_since_look = 0 # bypass the periodic poll again for this tick
        action = brain.decide()
        self.assertNotEqual(action, Action.BROADCAST)

    def test_navigates_toward_a_known_resource_when_idle(self):
        brain = make_brain(level=1) # needs 1 linemate, has none
        brain.state.known_resources["linemate"] = (7, 2)
        self.assertEqual(brain.decide(), Action.LEFT) # per move_toward_position toward (7, 2)

    def test_drops_stale_known_resource_once_arrived(self):
        brain = make_brain(level=1)
        brain.state.known_resources["linemate"] = (0, 0) # our own position, already empty
        brain.decide()
        self.assertNotIn("linemate", brain.state.known_resources)

    def test_ignores_a_known_resource_already_satisfied(self):
        brain = make_brain(level=1) # level 1 has no use for thystame
        brain.state.inventory.items["thystame"] = 1 # already holding one, target is 1
        brain.state.known_resources["thystame"] = (7, 2)
        brain.decide()
        self.assertEqual(brain.state.known_resources["thystame"], (7, 2)) # left untouched, not chased


if __name__ == "__main__":
    unittest.main()
