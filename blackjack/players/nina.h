#pragma once

#include "player.h"

class Nina : public Player
{
public:
    virtual Move make_move(const Card opponents_card, const Hand &its_hand) override;
    virtual std::string get_name() override;
};