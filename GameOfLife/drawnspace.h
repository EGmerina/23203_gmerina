#ifndef DRAWNSPACE_H
#define DRAWNSPACE_H

#include <memory>
#include <vector>
#include <QtWidgets/QGraphicsScene>

#include "circle.h"
#include "space.h"

class DrawnSpace
{
public:
    void drawSpace(Space *space, QGraphicsScene *scene);

private:
    std::vector<std::unique_ptr<Circle>> drawnspace;
};

#endif // DRAWNSPACE_H
