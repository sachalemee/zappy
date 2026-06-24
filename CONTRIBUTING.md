# Contributing to ZAPPY

Thanks for taking the time to contribute. This project is developed by a fixed team of six (Marin Jeunot, Evans Ramin, Thibault Miall, Daniel Lozovan, Nicolas Gonzalez and Sacha Lemée) for the Epitech YEP curriculum, but the same rules apply to everyone touching the code : including future us.

## Project layout

The repository ships three binaries (`zappy_server`, `zappy_gui`, `zappy_ai`) built from `src/server`, `src/gui` and `src/ai`. See [doc/PROJECT_STRUCTURE.md](doc/PROJECT_STRUCTURE.md) for the full breakdown, and [README.md](README.md) for build instructions.

## Branching

- Never push directly to `main`. Branch off it for every change.
- Name branches by area and topic, e.g. `server/eject-fix`, `AI/resource-sharing`, `gui/particle-system` : this repo's history follows that pattern, keep it consistent.
- Open a pull request into `main` once the branch is ready, the `zappy` GitHub Actions workflow must pass before merging.

## Commit messages

We follow a `type(scope): summary` convention, e.g. :

```
fix(SERVER): correct eject of eggs
feat(AI): add dfs algorithm
feat(GUI): add a useless box
test(AI): add protocol and connection tests
```

## Code style

- **Server (C)** : follow the Epitech C coding standard. No warnings under `-Wall -Wextra -Werror`.
- **GUI (C++)** : C++20, same warning policy. Keep rendering, network and game-state code in their respective subfolders under `src/gui/`.
- **AI (Python)** : no language constraint per the subject, we use Python 3.10 (at least) Keep the network layer (`src/ai/network/`), the game model (`src/ai/game/`), the decision algorithms (`src/ai/algo/`) and the orchestration (`src/ai/strategy/`) separate : don't let socket code leak into decision logic or vice versa.

## Pull requests

We follow a `SCOPE - Summary` convention, e.g. :

```
GUI - Fix general architecture
SERVER - Remove not working eggs
AI - Add dfs algorithm
```