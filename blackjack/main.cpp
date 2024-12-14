#include <iostream>
#include <string>
#include <vector>
#include <memory>
// #include <boost/program_options.hpp>
#include "factory.h"
#include "players/player.h"
#include "modes/mode.h"
#include "deckes/deck.h"

//./play mode=fast deck=simple masha dealer
// clang main.cpp masha.cpp dealer.cpp mode.cpp deck.cpp ndeck.cpp simpledeck.cpp detailedmode.cpp fastmode.cpp tournamentmode.cpp

// TODO boost:: program_options


// TODO тесты!!! написать фейковую колоду, которая всегда выдает одни и те же карты. для тестов
// написать фальшивые стратегии для тестирования движка - знаем кто выиграет

int main(int argc, char **argv)
{

    /* boost::program_options::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")("compression", po::value<int>(), "set compression level");
 */
    // перенаправить поток вывода из консоли в файл //stdout reopen...
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
