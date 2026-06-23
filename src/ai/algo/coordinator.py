##
## EPITECH PROJECT, 2026
## zappy
## File description:
## coordinator
##

# simple message protocol -> communicate with each others

INCANT_TAG = "ZAPPY"
INCANT_TYPE = "INCANT"
RESOURCE_TYPE = "RESOURCE"

class Coordinator:
    @staticmethod
    def encode_incant_call(team_name: str, level: int) -> str:
        return f"{INCANT_TAG};{team_name};{INCANT_TYPE};{level}"

    @staticmethod
    def parse_incant_call(team_name: str, text: str) -> int | None:
        parts = text.strip().split(";")
        if len(parts) != 4 or parts[0] != INCANT_TAG or parts[2] != INCANT_TYPE:
            return None
        if parts[1] != team_name:
            return None
        try:
            return int(parts[3])
        except ValueError:
            return None

    @staticmethod
    def encode_resource_report(team_name: str, resource: str, x: int, y: int) -> str:
        # broadcast only conveys a direction relative to the listener not the speaker
        return f"{INCANT_TAG};{team_name};{RESOURCE_TYPE};{resource};{x};{y}"

    @staticmethod
    def parse_resource_report(team_name: str, text: str) -> tuple[str, int, int] | None:
        parts = text.strip().split(";")
        if len(parts) != 6 or parts[0] != INCANT_TAG or parts[2] != RESOURCE_TYPE:
            return None
        if parts[1] != team_name:
            return None
        try:
            return parts[3], int(parts[4]), int(parts[5])
        except ValueError:
            return None

    @staticmethod
    def parse_broadcast_line(line: str) -> tuple[int, str] | None:
        # broadcast works like this : server sends "message K, <text>" to every player, K being the sound direction the text came from
        if not line.startswith("message "):
            return None
        header, sep, text = line.partition(",")
        if not sep:
            return None
        try:
            k = int(header.removeprefix("message ").strip())
        except ValueError:
            return None
        return k, text.strip()
