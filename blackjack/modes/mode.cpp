#include "mode.h"

std::unique_ptr<Player>::pointer Mode ::get_winner(const std::unique_ptr<Player> &player1, Hand &p1_hand, const std::unique_ptr<Player> &player2, Hand &p2_hand)
{
    if (p1_hand.get_points() == p2_hand.get_points() || (p1_hand.get_points() > 21 && p2_hand.get_points() > 21))
    {
        const std::unique_ptr<Player> nobody = nullptr;
        return nobody.get();
    }
    if (p1_hand.get_points() > 21)
    {
        return player2.get();
    }
    if (p2_hand.get_points() > 21)
    {
        return player1.get();
    }
    else if (p1_hand.get_points() > p2_hand.get_points())
    {
        return player1.get();
    }
    return player2.get();
}