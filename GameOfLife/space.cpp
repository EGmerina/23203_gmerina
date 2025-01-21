#include "space.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>

#define LOCALITY 3

Space::Space(size_t size) : size(size), space(new int[size * size]())
{
}
Space::~Space()
{
    delete[] space;
}

bool Space::willCellLive(size_t x, size_t y)
{
    return rule.canCellLive(getAmountOfCellsAround(x, y), isCellAlive(x, y));
}

bool Space::isCellAlive(size_t x, size_t y)
{
    return space[x + y * size] > 0;
}

void Space::removeCell(size_t x, size_t y)
{
    space[y * size + x] = 0;
}

void Space::addCell(size_t x, size_t y)
{
    space[y * size + x] = 1;
}

void Space::reload()
{
    std::fill_n(space, size * size, 0);
}

void Space::updateSpace()
{
    int *updatedspace = new int[size * size]();
    for (size_t y = 0; y < size; y++)
    {
        for (size_t x = 0; x < size; x++)
        {
            if (rule.canCellLive(getAmountOfCellsAround(x, y), isCellAlive(x, y)))
            {
                updatedspace[x + y * size] = 1;
            }
            else
            {
                updatedspace[x + y * size] = 0;
            }
        }
    }
    delete[] space;
    space = updatedspace;
}

size_t Space::getToroidalCoordinate(int coord)
{
    if (coord > static_cast<int>(size - 1))
    {

        return coord % size;
    }
    else if (coord < 0)
    {

        while (coord < 0)
        {
            coord += (static_cast<int>(size));
        }
        return coord;
    }
    else
    {

        return coord;
    }
}

size_t Space::getAmountOfCellsAround(size_t x, size_t y)
{
    size_t torX = 0;
    size_t torY = 0;
    size_t amountOfCells = 0;
    for (size_t nearY = 0; nearY < LOCALITY; nearY++)
    {
        for (size_t nearX = 0; nearX < LOCALITY; nearX++)
        {

            torX = getToroidalCoordinate(x - 1 + nearX);
            torY = getToroidalCoordinate(y - 1 + nearY);
            amountOfCells += space[torX + torY * size];
        }
    }
    amountOfCells -= space[x + y * size];
    return amountOfCells;
}

size_t Space::getSize()
{
    return size;
}
