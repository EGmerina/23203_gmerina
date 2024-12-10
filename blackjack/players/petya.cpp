#include "petya.h"

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

bool Petya ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if ((its_hand.have_card(1) || its_hand.have_card(11)) && (its_hand.have_card(7) || its_hand.have_card(8) || its_hand.have_card(9)))
    {
        return stand();
    }
    if (its_hand.get_card(0) == 10 && its_hand.get_card(1) == 10)
    {
        return stand();
    }
    if (its_hand.get_points() >= 17 || (its_hand.get_points() <= 16 && its_hand.get_points() >= 13 && opponents_card < 7))
    {
        return stand();
    }
    return hit();
}

std::string Petya ::get_name()
{
    std::string name = "petya";
    return name;
}

namespace
{
    RegisterCreator<Player, Petya> b("petya");
}
