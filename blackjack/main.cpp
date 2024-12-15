
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include "deckes/deck.h"
#include "factory.h"
#include "modes/mode.h"
#include "players/player.h"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Not enought arguments" << std::endl;
        return 0;
    }

    std::vector<std::string> names_of_players;
    std::string mode;

    po::options_description desc("Allowed options");
    desc.add_options()("help", "follow the instractions below")("players", po::value<std::vector<std::string>>(&names_of_players)->value_name("dealer masha nina petya vasya yan"), "set strategies")("mode", po::value<std::string>(&mode)->value_name("[detailed|fast|tournament]"), "set gamemode")("deck", po::value<std::string>()->value_name("[simple|N]")->default_value("simple"), "set deckmode")("N", po::value<int>()->value_name("[number of decks]")->default_value(4), "set N");

    po::positional_options_description pos;
    pos.add("strategies", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pos).run(), vm);
    po::notify(vm);

    if (vm.count("help") || names_of_players.size() < 2)
    {
        std::cout << desc << std::endl;
        return 1;
    }

    if (!vm.count("mode"))
    {
        mode = names_of_players.size() > 2 ? "tournamet" : "detailed";
    }

    if (names_of_players.size() > 2 && mode != "tournament")
    {
        std::cout << "mode tournament for 3 and more strategies" << std::endl;
        std::cout << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    auto gamemode = Factory<std::string, Mode>::getInstance()->createUnitByName(vm.at("mode").as<std::string>());
    auto deck = Factory<std::string, Deck>::getInstance()->createUnitByName(vm.at("deck").as<std::string>());

    std::vector<std::unique_ptr<Player>> players;
    for (auto name : names_of_players)
    {
        auto u = Factory<std::string, Player>::getInstance()->createUnitByName(name);
        players.push_back(std::move(u));
    }
    gamemode->play_game(std::move(players), std::move(deck));
}

//./play mode=fast deck=simple masha dealer
// clang main.cpp masha.cpp dealer.cpp mode.cpp deck.cpp ndeck.cpp simpledeck.cpp detailedmode.cpp fastmode.cpp tournamentmode.cpp

// TODO тесты!!! написать фейковую колоду, которая всегда выдает одни и те же карты. для тестов
// написать фальшивые стратегии для тестирования движка - знаем кто выиграет