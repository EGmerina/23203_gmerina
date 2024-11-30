#pragma once

#include "player.h"

class Masha : public Player
{
public:
    virtual bool make_move(Card opponents_card, Hand &its_hand) override;
    virtual std::string get_name() override;
};
