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

// нужно фабрику на режим игры и на выдачу карт . итого 3 фабрики
// написать фейковую колоду, которая всегда выдает одни и те же карты. для тестов
// написать фальшивые стратегии для тестирования движка - знаем кто выиграет

int main(int argc, char **argv)
{
    //перенаправить поток вывода из консоли в файл //stdout reopen...
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
