#pragma once
#include <memory>
#include <iostream>
#include "player.h"
#include "hand.h"

class ConsoleInterface
{
public:
    void output_points(std::shared_ptr<Player> player1, Hand &p1_hand)
    {
        std::cout << "points of " << player1->get_name() << " : " << p1_hand.get_points() << std::endl;
    }
    void announce_winner(std::shared_ptr<Player> winner)
    {
        std::cout << "winner is " << winner->get_name() << std::endl;
    }
};
