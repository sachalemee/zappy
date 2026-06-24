/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** gamestate
*/

#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

#include <string>
#include <vector>
#include <iosfwd>


struct Tile {
    int resources[7] = {0, 0, 0, 0, 0, 0, 0};
};

struct Player {
    int id = -1;
    int x = 0;
    int y = 0;
    int orientation = 1;
    int level = 1;
    std::string team;
    int inventory[7] = {0, 0, 0, 0, 0, 0, 0};
};

struct Egg {
    int id = -1;
    int x = 0;
    int y = 0;
};

class game_state {
public:
    bool handleLine(const std::string &line);
    void reset();
    bool ready() const { return _width > 0 && _height > 0; }
    int width() const { return _width; }
    int height() const { return _height; }
    int timeUnit() const { return _timeUnit; }
    const Tile &tile(int x, int y) const { return _tiles[y * _width + x]; }
    const std::vector<Player> &players() const { return _players; }
    const std::vector<Egg> &eggs() const { return _eggs; }
    const std::vector<std::string> &teams() const { return _teams; }

private:
    void parseMapSize(std::istringstream &args);
    void parseTimeUnit(std::istringstream &args);
    void parseTile(std::istringstream &args);
    void parseTeam(std::istringstream &args);
    void parseNewPlayer(std::istringstream &args);
    void parsePlayerPosition(std::istringstream &args);
    void parsePlayerLevel(std::istringstream &args);
    void parsePlayerInventory(std::istringstream &args);
    void parsePlayerDeath(std::istringstream &args);
    void parseNewEgg(std::istringstream &args);
    void parseEggRemoval(std::istringstream &args);
    Player *findPlayer(int id);
    static int extractId(const std::string &token);

    int _width = 0;
    int _height = 0;
    int _timeUnit = 100;
    std::vector<Tile> _tiles;
    std::vector<std::string> _teams;
    std::vector<Player> _players;
    std::vector<Egg> _eggs;

};


#endif /* !GAMESTATE_HPP_ */
