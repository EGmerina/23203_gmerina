#pragma once
#include <memory>
#include <iostream>
#include "players/player.h"
#include "hand.h"

class ConsoleInterface
{
public:
    void output_points(std::shared_ptr<Player> player1, Hand &p1_hand)
    {
        std::cout << "points of " << player1->get_name() << " : " << std::to_string(p1_hand.get_points()) << std::endl;
    }
    void output_vict_num(std::shared_ptr<Player> player1, size_t amount_vict)
    {
        std::cout << "amount victories of " << player1->get_name() << " : " << amount_vict << std::endl;
    }
    void announce_winner(std::shared_ptr<Player> winner)
    {
        if (nullptr == winner)
        {
            std::cout << "draw!" << std::endl;
            return;
        }
        std::cout << "winner is " << winner->get_name() << std::endl;
    }
    void announce_players(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
    {
        std::cout << "players are " << player1->get_name() << " and " << player2->get_name() << std::endl;
    }
};
