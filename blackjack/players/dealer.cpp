
#include "dealer.h"

#include <string>
#include "../register_creator.h"

static bool hit()
{
    return true;
}

static bool stand()
{
    return false;
}

bool Dealer ::make_move(Card opponents_card, Hand &its_hand)
{
    if (its_hand.get_points() < 17)
    {
        return hit();
    }
    return stand();
}

std::string Dealer ::get_name()
{
    std::string name = "dealer";
    return name;
}

namespace
{
    RegisterCreator<Player, Dealer> b("dealer");
}
