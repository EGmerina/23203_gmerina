#pragma once
#include <vector>
#include <utility>
#include "player.h"

// движок должен запоминать карты
//  написать функцию которая выводит имя игрока
// не хранить очки в player. хранить здесь. вообще можно было сделать protected наследование
// игроки берут карты по порядку
// написать class user interface, в котором будут взаимодействия с пользователем и вызывать эти функции
// в тесте подавать фейковый интерфейс, который будет содержать expects. использовать gmock!!!

static void generate_opponents(std::vector<std::pair<Player, Player>> pairs, std::vector<std::unique_ptr<Player>> players)
{
    for (size_t first = 0; first < players.size() - 1; first++)
    {
        for (size_t second = first; second < players.size(); second++)
        {
            std::pair<Player, Player> new_pair(players[first], players[second]);
            pairs.push_back(new_pair);
        }
    }
}

void play_game(std::vector<std::unique_ptr<Player>> players)
{
    for (auto &u : players) // auto &
    {
        u->get_new_card();
        // распечатать лицевую карту
        u->get_new_card();
    }
    // выбрать всевозможные двойки игроков
    std::vector<std::pair<Player, Player>> pairs;
    generate_opponents(pairs, players); // паросочетания без повторений
    for (auto u : pairs)
    {
        u.first.play(u.second.get_face_card());
        u.second.play(u.first.get_face_card());
        // вывести кто выиграл
    }
}
