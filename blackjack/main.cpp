#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "factory.h"
#include "player.h"
#include "mode.h"
#include "deck.h"

//./play mode=fast deck=simple masha dealer
// g++ main.cpp dealer.cpp deck.cpp mode.cpp masha.cpp

// getopt
// boost:: program_options
// CATPZ
// TCLAP

// нужно фабрику на режим игры и на выдачу карт . итого 3 фабрики

// нужно написать дефолтные режимы игры!!!!!

// написать фейковую колоду, которая всегда выдает одни и те же карты. для тестов
// написать фальшивые стратегии для тестирования движка - знаем кто выиграет

int main(int argc, char **argv)
{
    // перенаправить поток вывода из консоли в файл //stdout reopen...
    if (argc < 3)
    {
        std::cerr << "gimme players" << std::endl;
        return -1;
    }

    // пока что сделаю попроще и буду считать что первый аргумент - это всегда режим , а второй - вариант колоды

    auto mode = Factory<std::string, Mode>::getInstance()->createUnitByName(argv[1]); // тут argv[1] !!!!!!!!!!!!!!!!!!!!

    auto deck = Factory<std::string, Deck>::getInstance()->createUnitByName(argv[2]);

    std::vector<std::shared_ptr<Player>> players;
    for (int i = 3; i < argc; i++)
    {
        auto u = Factory<std::string, Player>::getInstance()->createUnitByName(argv[i]); // написать деструктор для объектов?
        players.push_back(std::move(u));
    }
    mode->play_game(players, deck);
}
