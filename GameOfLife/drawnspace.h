#ifndef DRAWNSPACE_H
#define DRAWNSPACE_H
#include "space.h"
#include "circle.h"


class DrawnSpace
{
public:
    DrawnSpace(Space * space) : space(space){

    }

private:
    Space* space;
    Circle * drawnspace;
    size_t size;
};

#endif // DRAWNSPACE_H
