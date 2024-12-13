#pragma once

#include "player.h"

class Yan : public Player
{
public:
    virtual bool make_move(const Card opponents_card, const Hand &its_hand) override;
    virtual std::string get_name() override;
};