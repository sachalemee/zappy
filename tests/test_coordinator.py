##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_coordinator
##

import unittest

from src.ai.algo.coordinator import Coordinator


class TestEncodeParseIncantCall(unittest.TestCase):
    def test_round_trip(self):
        text = Coordinator.encode_incant_call("team1", 3, 1234)
        self.assertEqual(Coordinator.parse_incant_call("team1", text), (3, 1234))

    def test_none_when_wrong_team(self):
        text = Coordinator.encode_incant_call("team1", 3, 1234)
        self.assertIsNone(Coordinator.parse_incant_call("team2", text))

    def test_none_when_unrelated_text(self):
        self.assertIsNone(Coordinator.parse_incant_call("team1", "hello there"))


class TestEncodeParseResourceReport(unittest.TestCase):
    def test_round_trip(self):
        text = Coordinator.encode_resource_report("team1", "thystame", 4, 7)
        self.assertEqual(Coordinator.parse_resource_report("team1", text), ("thystame", 4, 7))

    def test_none_when_wrong_team(self):
        text = Coordinator.encode_resource_report("team1", "thystame", 4, 7)
        self.assertIsNone(Coordinator.parse_resource_report("team2", text))

    def test_none_when_unrelated_text(self):
        self.assertIsNone(Coordinator.parse_resource_report("team1", "hello there"))

    def test_none_when_confused_with_incant_call(self):
        text = Coordinator.encode_incant_call("team1", 3, 1234)
        self.assertIsNone(Coordinator.parse_resource_report("team1", text))


class TestParseBroadcastLine(unittest.TestCase):
    def test_parses_direction_and_text(self):
        self.assertEqual(Coordinator.parse_broadcast_line("message 3, hello"), (3, "hello"))

    def test_none_when_not_a_message(self):
        self.assertIsNone(Coordinator.parse_broadcast_line("ok"))

    def test_none_when_malformed(self):
        self.assertIsNone(Coordinator.parse_broadcast_line("message abc, hello"))


if __name__ == "__main__":
    unittest.main()
