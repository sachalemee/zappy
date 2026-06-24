##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_protocol
##

import socket
import unittest
from unittest.mock import MagicMock

from src.ai.network.protocol import Protocol
from src.ai.strategy.actions import Action


def make_protocol() -> Protocol:
    connection = MagicMock()
    connection.socket = MagicMock()
    return Protocol(connection)


class TestHandshake(unittest.TestCase):
    def test_success_returns_width_and_height(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["WELCOME", "2", "10 15"]
        self.assertEqual(protocol.handshake("team1"), (10, 15))
        protocol.connection.send.assert_called_once_with("team1")

    def test_raises_when_welcome_is_missing(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["nope"]
        with self.assertRaises(ConnectionError):
            protocol.handshake("team1")

    def test_raises_when_client_num_is_none(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["WELCOME", None]
        with self.assertRaises(ConnectionError):
            protocol.handshake("team1")

    def test_raises_when_size_is_none(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["WELCOME", "2", None]
        with self.assertRaises(ConnectionError):
            protocol.handshake("team1")

    def test_raises_when_team_is_rejected_with_ko(self):
        # server now writes "ko\n" before closing the socket on a full/unknown team
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["WELCOME", "ko"]
        with self.assertRaises(ConnectionError):
            protocol.handshake("team1")


class TestSendCommand(unittest.TestCase):
    def test_sends_the_command_and_queues_the_action(self):
        protocol = make_protocol()
        protocol.send_command(Action.TAKE, "food")
        protocol.connection.send.assert_called_once_with("Take food")
        self.assertEqual(list(protocol.pending), [Action.TAKE])

    def test_queues_in_order_across_multiple_sends(self):
        protocol = make_protocol()
        protocol.send_command(Action.FORWARD)
        protocol.send_command(Action.LOOK)
        self.assertEqual(list(protocol.pending), [Action.FORWARD, Action.LOOK])


class TestPollResponses(unittest.TestCase):
    def test_matches_a_response_to_the_pending_action(self):
        protocol = make_protocol()
        protocol.pending.append(Action.FORWARD)
        protocol.connection.receive_line.side_effect = ["ok", None]
        self.assertEqual(protocol.poll_responses(), [(Action.FORWARD, "ok")])
        self.assertEqual(list(protocol.pending), [])

    def test_pushed_lines_have_no_action(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["message 3, hello", None]
        self.assertEqual(protocol.poll_responses(), [(None, "message 3, hello")])

    def test_dead_is_a_pushed_line(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["dead", None]
        self.assertEqual(protocol.poll_responses(), [(None, "dead")])

    def test_current_level_is_pushed_when_not_awaiting_incantation(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["Current level: 3", None]
        self.assertEqual(protocol.poll_responses(), [(None, "Current level: 3")])

    def test_current_level_matches_pending_incantation(self):
        protocol = make_protocol()
        protocol.pending.append(Action.INCANTATION)
        protocol.connection.receive_line.side_effect = ["Current level: 3", None]
        self.assertEqual(protocol.poll_responses(), [(Action.INCANTATION, "Current level: 3")])

    def test_stops_on_socket_timeout(self):
        protocol = make_protocol()
        protocol.pending.append(Action.LOOK)
        protocol.connection.receive_line.side_effect = socket.timeout()
        self.assertEqual(protocol.poll_responses(), [])

    def test_resets_the_timeout_after_polling(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = [None]
        protocol.poll_responses()
        protocol.connection.socket.settimeout.assert_called_with(None)

    def test_unmatched_response_without_pending_action_is_dropped(self):
        protocol = make_protocol()
        protocol.connection.receive_line.side_effect = ["ok", None]
        self.assertEqual(protocol.poll_responses(), [])


if __name__ == "__main__":
    unittest.main()
