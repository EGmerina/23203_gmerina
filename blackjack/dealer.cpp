#include "factory.h"
#include "dealer.h"
//#include "deck.h"

#include <string>

static bool hit(Hand &its_hand)
{
    return true; //??? где получить карту??
}

static bool stand()
{
    return false;
}

bool Dealer ::make_move(Card opponents_card, Hand &its_hand)
{
    if (its_hand.get_points() < 17)
    {
        return hit(its_hand);
    }
    return stand();
}

std::string Dealer ::get_name()
{
}

Player *createDealer()
{
    return new Dealer();
}
namespace
{
    bool b = Factory<std::string, Player>::getInstance()->registerCreator("dealer", createDealer);
}
