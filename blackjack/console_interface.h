#pragma once
#include <iostream>
#include <memory>
#include "hand.h"
#include "players/player.h"

class ConsoleInterface
{
public:
    void output_points(const std::unique_ptr<Player> &player1, Hand &p1_hand)
    {
        std::cout << "points of " << player1->get_name() << " : " << std::to_string(p1_hand.get_points()) << std::endl;
    }

    void announce_whos_turn(const std::unique_ptr<Player> &player)
    {
        std::cout << "turn of " << player->get_name() << std::endl;
    }

    void output_vict_num(const std::unique_ptr<Player> &player1, size_t amount_vict)
    {
        std::cout << "amount victories of " << player1->get_name() << " : " << amount_vict << std::endl;
    }

    void announce_winner(const std::unique_ptr<Player> &winner)
    {
        if (nullptr == winner)
        {
            std::cout << "draw!" << std::endl;
            return;
        }
        std::cout << "winner is " << winner->get_name() << std::endl;
    }

    void announce_new_round(const std::unique_ptr<Player> &player1, const std::unique_ptr<Player> &player2)
    {
        std::cout << "######### NEW ROUND ##########" << std::endl;
        std::cout << "players are " << player1->get_name() << " and " << player2->get_name() << std::endl;
    }

    void announce_total_protocol()
    {
        std::cout << "######### TOTAL PROTOCOL ##########" << std::endl;
    }

    void announce_move(bool move)
    {
        if (move)
        {
            std::cout << "HIT" << std::endl;
            return;
        }
        std::cout << "STAND" << std::endl;
    }
};
