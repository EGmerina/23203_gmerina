#pragma once
#include <stdio.h>
#include <algorithm>

class Space
{
public:
    Space(size_t size = 10)
    {
    }
    ~Space()
    {
    }

    void updateSpace()
    {
    }

    void removeCell(size_t x, size_t y)
    {
    }

    void addCell(size_t x, size_t y)
    {
    }

    void reload()
    {
    }

    size_t getToroidalCoordinate(int coord) {}

    size_t getAmountOfCellsAround(size_t x, size_t y) {}

private:
    size_t size = 0;
    int *space;
};