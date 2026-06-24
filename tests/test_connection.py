##
## EPITECH PROJECT, 2026
## zappy
## File description:
## test_connection
##

import unittest
from unittest.mock import MagicMock

from src.ai.network.connection import Connection


def make_connection() -> Connection:
    connection = Connection("localhost", 4242)
    connection.socket = MagicMock()
    return connection


class TestSend(unittest.TestCase):
    def test_sends_the_line_with_a_trailing_newline(self):
        connection = make_connection()
        connection.send("Forward")
        connection.socket.sendall.assert_called_once_with(b"Forward\n")

    def test_does_nothing_once_closed_by_peer(self):
        connection = make_connection()
        connection.closed_by_peer = True
        connection.send("Forward")
        connection.socket.sendall.assert_not_called()

    def test_marks_closed_by_peer_on_connection_error(self):
        connection = make_connection()
        connection.socket.sendall.side_effect = ConnectionError()
        connection.send("Forward")
        self.assertTrue(connection.closed_by_peer)


class TestReceiveLine(unittest.TestCase):
    def test_returns_a_single_buffered_line(self):
        connection = make_connection()
        connection.socket.recv.return_value = b"WELCOME\n"
        self.assertEqual(connection.receive_line(), "WELCOME")

    def test_strips_whitespace(self):
        connection = make_connection()
        connection.socket.recv.return_value = b"  ok  \n"
        self.assertEqual(connection.receive_line(), "ok")

    def test_assembles_a_line_split_across_chunks(self):
        connection = make_connection()
        connection.socket.recv.side_effect = [b"WEL", b"COME\n"]
        self.assertEqual(connection.receive_line(), "WELCOME")

    def test_buffers_a_second_line_for_the_next_call(self):
        connection = make_connection()
        connection.socket.recv.return_value = b"ok\nko\n"
        self.assertEqual(connection.receive_line(), "ok")
        self.assertEqual(connection.receive_line(), "ko")

    def test_none_and_closed_by_peer_on_graceful_close(self):
        connection = make_connection()
        connection.socket.recv.return_value = b""
        self.assertIsNone(connection.receive_line())
        self.assertTrue(connection.closed_by_peer)

    def test_none_and_closed_by_peer_on_connection_error(self):
        connection = make_connection()
        connection.socket.recv.side_effect = ConnectionError()
        self.assertIsNone(connection.receive_line())
        self.assertTrue(connection.closed_by_peer)


class TestClose(unittest.TestCase):
    def test_closes_and_clears_the_socket(self):
        connection = make_connection()
        sock = connection.socket
        connection.close()
        sock.close.assert_called_once()
        self.assertIsNone(connection.socket)

    def test_safe_to_call_when_already_closed(self):
        connection = make_connection()
        connection.close()
        connection.close() # should not raise


if __name__ == "__main__":
    unittest.main()
