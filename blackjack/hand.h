#pragma once
#include <vector>
#include <algorithm>

using Card = unsigned char;

class Hand
{
public:
    const Card &get_face_card() const
    {
        return hand[0];
    }

    const Card &get_card(const unsigned char num_card) const
    {
        if (num_card < hand.size() - 1)
        {
            return hand[num_card];
        }
        return hand[hand.size() - 1];
    }

    void add_card(const Card new_card)
    {
        hand.push_back(new_card);
        points += new_card;
    }

    unsigned char get_points() const
    {
        return points;
    }

    bool have_card(const Card &card) const
    {
        if (std::find(hand.begin(), hand.end(), card) != hand.end())
        {
            return true;
        }
        return false;
    }

private:
    std::vector<Card> hand; // лицевая карта под индексом 0
    unsigned char points = 0;
};
