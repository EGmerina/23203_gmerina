#include "rules.h"

bool Rules::canCellLive(size_t amount_cells_around, bool isalive)
{
    if (amount_cells_around > 3 || amount_cells_around < 2)
    {
        return false;
    }
    if (amount_cells_around != 3 && !isalive)
    {
        return false;
    }
    return true;
}
