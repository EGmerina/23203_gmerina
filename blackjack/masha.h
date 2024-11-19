#pragma once

#include "player.h"

class Masha : public Player
{
    virtual void hit() override;
    virtual void stand() override;
    virtual void play() override;
};
