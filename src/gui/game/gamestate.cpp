/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gamestate
*/

#include "game/gamestate.hpp"
#include <sstream>
#include <unordered_map>

int game_state::extractId(const std::string &token)
{
    if (token.size() > 1 && token[0] == '#')
        return std::stoi(token.substr(1));
    return std::stoi(token);
}

Player *game_state::findPlayer(int id)
{
    for (auto &player : _players)
        if (player.id == id)
            return &player;
    return nullptr;
}

void game_state::parseMapSize(std::istringstream &args)
{
    args >> _width >> _height;
    _tiles.assign(static_cast<size_t>(_width) * _height, Tile{});
}

void game_state::parseTimeUnit(std::istringstream &args)
{
    args >> _timeUnit;
}

void game_state::parseTile(std::istringstream &args)
{
    int x = 0;
    int y = 0;

    args >> x >> y;
    if (x < 0 || y < 0 || x >= _width || y >= _height)
        return;
    for (int i = 0; i < 7; i++)
        args >> _tiles[y * _width + x].resources[i];
}

void game_state::parseTeam(std::istringstream &args)
{
    std::string name;

    args >> name;
    _teams.push_back(name);
}

void game_state::parseNewPlayer(std::istringstream &args)
{
    Player player;
    std::string token;

    args >> token >> player.x >> player.y >> player.orientation
        >> player.level >> player.team;
    player.id = extractId(token);
    _players.push_back(player);
}

void game_state::parsePlayerPosition(std::istringstream &args)
{
    std::string token;
    int x = 0;
    int y = 0;
    int orientation = 0;
    Player *player = nullptr;

    args >> token >> x >> y >> orientation;
    player = findPlayer(extractId(token));
    if (player != nullptr) {
        player->x = x;
        player->y = y;
        player->orientation = orientation;
    }
}

void game_state::parsePlayerLevel(std::istringstream &args)
{
    std::string token;
    int level = 0;
    Player *player = nullptr;

    args >> token >> level;
    player = findPlayer(extractId(token));
    if (player != nullptr)
        player->level = level;
}

void game_state::parsePlayerDeath(std::istringstream &args)
{
    std::string token;
    int id = 0;

    args >> token;
    id = extractId(token);
    std::erase_if(_players, [id](const Player &p) { return p.id == id; });
}

void game_state::parseNewEgg(std::istringstream &args)
{
    Egg egg;
    std::string eggToken;
    std::string playerToken;

    args >> eggToken >> playerToken >> egg.x >> egg.y;
    egg.id = extractId(eggToken);
    _eggs.push_back(egg);
}

void game_state::parseEggRemoval(std::istringstream &args)
{
    std::string token;
    int id = 0;

    args >> token;
    id = extractId(token);
    std::erase_if(_eggs, [id](const Egg &e) { return e.id == id; });
}

bool game_state::handleLine(const std::string &line)
{
    static const std::unordered_map<std::string,
        void (game_state::*)(std::istringstream &)> handlers = {
        {"msz", &game_state::parseMapSize},
        {"sgt", &game_state::parseTimeUnit},
        {"sst", &game_state::parseTimeUnit},
        {"bct", &game_state::parseTile},
        {"tna", &game_state::parseTeam},
        {"pnw", &game_state::parseNewPlayer},
        {"ppo", &game_state::parsePlayerPosition},
        {"plv", &game_state::parsePlayerLevel},
        {"pdi", &game_state::parsePlayerDeath},
        {"enw", &game_state::parseNewEgg},
        {"ebo", &game_state::parseEggRemoval},
        {"edi", &game_state::parseEggRemoval},
    };
    std::istringstream stream(line);
    std::string command;

    stream >> command;
    auto handler = handlers.find(command);
    if (handler == handlers.end())
        return false;
    (this->*(handler->second))(stream);
    return true;
}