#include "drawnspace.h"

#define GAP_RATIO 0.93

void DrawnSpace::drawSpace(Space *space, QGraphicsScene *scene)
{

    drawnspace.clear();
    size_t sizeofscene = (scene->width() < scene->height()) ? scene->width() : scene->height();
    qreal distbtwcells = (sizeofscene / static_cast<qreal>(space->getSize()));
    qreal r = distbtwcells * GAP_RATIO;
    for (size_t y = 0; y < space->getSize(); ++y)
    {
        for (size_t x = 0; x < space->getSize(); ++x)
        {
            std::unique_ptr<Circle> c = std::make_unique<Circle>(distbtwcells * x, distbtwcells * y, r, scene);
            c->setSpace(space);
            c->setPosition(x, y);
            if (space->isCellAlive(x, y))
            {
                c->setBrush(QBrush(Qt::red));
            }
            else
            {

                c->setBrush(QBrush(Qt::gray));
            }
            drawnspace.push_back(std::move(c));
        }
    }
}
