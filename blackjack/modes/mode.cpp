#include "mode.h"

static std::unique_ptr<Player> get_winner(std::unique_ptr<Player> player1, Hand &p1_hand, std::unique_ptr<Player> player2, Hand &p2_hand)
{
    if (p1_hand.get_points() == p2_hand.get_points() || (p1_hand.get_points() > 21 && p2_hand.get_points() > 21))
    {
        return nullptr;
    }
    if (p1_hand.get_points() > 21)
    {
        return std::move(player2);
    }
    if (p2_hand.get_points() > 21)
    {
        return std::move(player1);
    }
    else if (p1_hand.get_points() > p2_hand.get_points())
    {
        return std::move(player1);
    }
    return std::move(player2);
}
