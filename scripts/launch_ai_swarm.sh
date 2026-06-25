#!/bin/bash
##
## EPITECH PROJECT, 2026
## zappy
## File description:
## launch_ai_swarm
##
## Starts a zappy_server and connects N AI clients split across two teams,
## to manually stress-test forking, broadcast coordination and incantations
## with several bots in the same world.
##
## Usage: ./scripts/launch_ai_swarm.sh [total_ai] [team1] [team2] [port] [width] [height] [freq]
##

set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR" || exit 84

TOTAL_AI="${1:-20}"
TEAM1="${2:-team1}"
TEAM2="${3:-team2}"
PORT="${4:-4242}"
WIDTH="${5:-20}"
HEIGHT="${6:-20}"
FREQ="${7:-100}"
HOST="localhost"

if [ "$((TOTAL_AI % 2))" -ne 0 ]; then
    echo "total_ai must be even (split evenly across the two teams)"
    exit 84
fi

AI_PER_TEAM=$((TOTAL_AI / 2))

if [ ! -x ./zappy_server ] || [ ! -x ./zappy_ai ]; then
    echo "zappy_server and/or zappy_ai not found at repo root ; build the project first"
    exit 84
fi

LOG_DIR="$ROOT_DIR/scripts/logs/$(date +%Y%m%d_%H%M%S)"
mkdir -p "$LOG_DIR"

PIDS=()

cleanup() {
    echo ""
    echo "Stopping server and ${#PIDS[@]} AI client(s)..."
    for pid in "${PIDS[@]}"; do
        kill "$pid" 2>/dev/null
    done
    wait 2>/dev/null
}
trap cleanup EXIT INT TERM

echo "Starting zappy_server on port $PORT (${WIDTH}x${HEIGHT}, freq=$FREQ, $AI_PER_TEAM initial slot(s)/team)..."
./zappy_server -p "$PORT" -x "$WIDTH" -y "$HEIGHT" -n "$TEAM1" "$TEAM2" -c "$AI_PER_TEAM" -f "$FREQ" \
    > "$LOG_DIR/server.log" 2>&1 &
PIDS+=("$!")

sleep 1

if ! kill -0 "${PIDS[0]}" 2>/dev/null; then
    echo "zappy_server failed to start, check $LOG_DIR/server.log"
    exit 84
fi

launch_team() {
    local team="$1"
    local count="$2"

    for i in $(seq 1 "$count"); do
        ./zappy_ai -p "$PORT" -n "$team" -h "$HOST" \
            > "$LOG_DIR/${team}_${i}.log" 2>&1 &
        PIDS+=("$!")
    done
}

echo "Launching $AI_PER_TEAM AI(s) for team '$TEAM1' and $AI_PER_TEAM for team '$TEAM2'..."
launch_team "$TEAM1" "$AI_PER_TEAM"
launch_team "$TEAM2" "$AI_PER_TEAM"

echo "$TOTAL_AI AI client(s) connected to $HOST:$PORT. Logs in $LOG_DIR/"
echo "Press Ctrl+C to stop everything."

wait
