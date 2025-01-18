#include "space.h"
#include <stdio.h>
#include <algorithm>
#include "rules.h"


#define LOCALITY 3

Space::Space(size_t size) : size(size), space(new int[size * size])
{
}
Space::~Space()
{
    delete[] space;
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
    Rules rule;
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (rule.canCellLive(getAmountOfCellsAround(i, j), space[i + j * size] > 0))
            {
                space[i + j * size] = 1;
            }
            else
            {
                space[i + j * size] = 0;
            }
        }
    }
}

size_t Space::getToroidalCoordinate(int coord)
{
    return coord % size; //TODO change %
}

size_t Space::getAmountOfCellsAround(size_t x, size_t y)
{
    size_t X = 0;
    size_t Y = 0;
    size_t amountOfCells = 0;
    for (size_t i = 0; i < LOCALITY; i++)
    {
        for (size_t j = 0; j < LOCALITY; j++)
        {

            X = getToroidalCoordinate(x - 1 + i);
            Y = getToroidalCoordinate(y - 1 + j);
            amountOfCells += space[X + Y * size];
        }
    }
    amountOfCells -= space[x + y * size];
    return amountOfCells;
}

size_t Space::getSize(){
    return size;
}

bool Space::isCellAlive(size_t x, size_t y){
    return space[x+y*size]>0;
}

void Space:: editCell(size_t x, size_t y){
    space[x+ y*size] = 1 - space[x+ y*size];
}
