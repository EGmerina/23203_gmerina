#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "engine.h"
#include "factory.h"
#include "player.h"

// getopt
// boost:: program_options
// CATPZ
// TCLAP

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "gimme players" << std::endl;
        return -1;
    }

    std::vector<std::unique_ptr<Player>> players;
    for (int i = 0; i < argc; i++)
    {
        auto u = Factory<std::string, Player>::getInstance()->createUnitByName(argv[i]);
        players.push_back(std::move(u));
    }
    play_game(std::move(players));
}
