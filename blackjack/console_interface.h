#pragma once
#include <memory>
#include "player.h"
#include "hand.h"

class ConsoleInterface
{
public:
    void output_points(std::shared_ptr<Player> player1, Hand &p1_hand)
    {
    }
    void announce_winner(std::shared_ptr<Player> winner)
    {
    }
};
