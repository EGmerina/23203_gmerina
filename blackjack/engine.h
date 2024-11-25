#pragma once
#include <vector>
#include <utility>
#include "player.h"
#include "mode.h"
#include "deck.h"

//КАК-ТО СТРАННО ВЫЗЫВАТЬ В MAIN PLAY_GAME, А ЗДЕСЬ MODE.PLAY_GAME(). ПОЭТОМУ ЕСТЬ ОЩУЩЕНИЕ ЧТО ДВИЖОК НЕ НУЖЕН

#define INITIAL_CARDS_NUMBER 2

// движок должен запоминать карты
//  написать функцию которая выводит имя игрока
// не хранить очки в player. хранить здесь. вообще можно было сделать protected наследование
// игроки берут карты по порядку
// написать class user interface, в котором будут взаимодействия с пользователем и вызывать эти функции
// в тесте подавать фейковый интерфейс, который будет содержать expects. использовать gmock!!!

static void generate_opponents(std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>> pairs, std::vector<std::shared_ptr<Player>> players)
{
    for (size_t first = 0; first < players.size() - 1; first++)
    {
        for (size_t second = first; second < players.size(); second++)
        {
            std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>> new_pair{players[first], players[second]};
            pairs.push_back(new_pair);
        }
    }
}

static void play_tour(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
{
    unsigned char stand_cnt = 0;
    while (stand_cnt < 2)
    {
        Hand p1;
        Hand p2;
    }
}

void play_game(std::vector<std::shared_ptr<Player>> players, std::unique_ptr<Mode> mode, std::unique_ptr<Deck> deck)
{

    // выбрать всевозможные двойки игроков
    std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>> pairs;
    generate_opponents(pairs, players);

    for (auto &u : pairs)
    {
        play_tour(u.first, u.second);
    }
}
