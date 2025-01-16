#pragma once
#include "space.h"
#include <qt5/QtWidgets/QAbstractButton>



void play(Space space)
{
    while (!play)
    {
        space.addCell();
    }

    while (!stop)
    {
        space.updateSpace();
        sleep();
    }
}

void draw_game()
{
    Space space;
    while (!end)
    {
        draw(space);
        play(space);
        if (reload)
        {
            space.reload();
        }
    }
}
