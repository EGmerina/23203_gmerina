#include <iostream>
#include <time.h>
#include "../hand.h"

#define Jack 10
#define Queen 10
#define King 10

constexpr static Card standart_deck[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, 11};

static size_t get_random_num(const size_t beg, const size_t end)
{
    srand(time(0));
    return rand() % (end - beg + 1) + beg;
}
