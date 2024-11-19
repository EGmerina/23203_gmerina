#pragma once
#include <vector>
#include <utility>
#include "player.h"

// написать функцию которая выводит имя игрока

void play_game(std::vector<std::unique_ptr<Player>> players)
{
    for (auto &u : players) // что это такое???? но работает...наверное
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
