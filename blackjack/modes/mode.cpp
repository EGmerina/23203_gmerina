#include "mode.h"

static const std::unique_ptr<Player> &get_winner(const std::unique_ptr<Player> &player1, Hand &p1_hand, const std::unique_ptr<Player> &player2, Hand &p2_hand)
{
    if (p1_hand.get_points() == p2_hand.get_points() || (p1_hand.get_points() > 21 && p2_hand.get_points() > 21))
    {
        const std::unique_ptr<Player> &nobody = nullptr; // TODO !!!!!
        return nobody;
    }
    if (p1_hand.get_points() > 21)
    {
        return player2;
    }
    if (p2_hand.get_points() > 21)
    {
        return player1;
    }
    else if (p1_hand.get_points() > p2_hand.get_points())
    {
        return player1;
    }
    return player2;
}
