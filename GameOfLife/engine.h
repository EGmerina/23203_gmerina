#ifndef ENGINE_H
#define ENGINE_H
#include "space.h"




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

#endif // ENGINE_H
