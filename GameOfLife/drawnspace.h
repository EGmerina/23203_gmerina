#ifndef DRAWNSPACE_H
#define DRAWNSPACE_H
#include "space.h"
#include <QtWidgets/QGraphicsScene>
#include <vector>
#include <memory>
#include "circle.h"


class DrawnSpace
{
public:

    void  drawSpace(Space * space, QGraphicsScene* scene);

private:

    std::vector<std:: unique_ptr<Circle>> drawnspace;

};

#endif // DRAWNSPACE_H
