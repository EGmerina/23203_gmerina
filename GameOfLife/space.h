#ifndef SPACE_H
#define SPACE_H

#include <algorithm>
#include <stdio.h>
#include "rules.h"

class Space
{
public:
    Space(size_t size = 10);
    ~Space();

    void updateSpace();

    void removeCell(size_t x, size_t y);

    void addCell(size_t x, size_t y);

    void reload();

    size_t getAmountOfCellsAround(size_t x, size_t y);

    size_t getSize();

    bool isCellAlive(size_t x, size_t y);

    bool willCellLive(size_t x, size_t y);

private:
    size_t size = 0;
    int *space;
    Rules rule;

    size_t getToroidalCoordinate(int coord);
};
#endif // SPACE_H
