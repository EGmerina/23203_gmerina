#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

// #include <boost/program_options.hpp>
#include "deckes/deck.h"
#include "factory.h"
#include "modes/mode.h"
#include "players/player.h"

int main(int argc, char **argv)
{

    if (!freopen("output.txt", "w", stdout))
    {
        perror("Unable to reopen stdout");
        return 1;
    }

    if (argc < 4)
    {
        std::cerr << "gimme players" << std::endl;
        return -1;
    }

    auto mode = Factory<std::string, Mode>::getInstance()->createUnitByName(argv[1]);

    auto deck = Factory<std::string, Deck>::getInstance()->createUnitByName(argv[2]);

    std::vector<std::shared_ptr<Player>> players;
    for (int i = 3; i < argc; i++)
    {
        auto u = Factory<std::string, Player>::getInstance()->createUnitByName(argv[i]);
        players.push_back(std::move(u));
    }
    mode->play_game(players, deck);
}

/*    boost::program_options::options_description desc("Allowed options");
   desc.add_options()("help", "produce help message")("compression", boost::program_options::value<int>(), "set compression level"); */

//./play mode=fast deck=simple masha dealer
// clang main.cpp masha.cpp dealer.cpp mode.cpp deck.cpp ndeck.cpp simpledeck.cpp detailedmode.cpp fastmode.cpp tournamentmode.cpp

// TODO boost:: program_options

// TODO тесты!!! написать фейковую колоду, которая всегда выдает одни и те же карты. для тестов
// написать фальшивые стратегии для тестирования движка - знаем кто выиграет