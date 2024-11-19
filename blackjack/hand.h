#pragma once
#include <vector>

using Card = unsigned char;

class Hand
{
public:
    Card &get_face_card()
    {
        return hand[0];
    }

    void add_card(const Card new_card)
    {
        hand.push_back(new_card);
    }

private:
    std::vector<Card> hand; // лицевая карта под индексом 0
    unsigned char points = 0;
};
