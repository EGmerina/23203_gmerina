#include "mode.h"

static std::shared_ptr<Player> get_winner(std::shared_ptr<Player> player1, Hand &p1_hand, std::shared_ptr<Player> player2, Hand &p2_hand)
{
    if (p1_hand.get_points() > 21)
    {
        return player2;
    }
    if (p2_hand.get_points() > 21)
    {
        return player1;
    }
    if (p1_hand.get_points() == p2_hand.get_points())
    {
        return NULL;
    }
    else if (p1_hand.get_points() > p2_hand.get_points()) // TODO дописать с проверкой карт на блэкджек
    {
        return player1;
    }
    return player2;
}
