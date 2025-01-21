#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsScene>
#include "circle.h"

class CustomGraphicsScene : public QGraphicsScene
{
public:
    CustomGraphicsScene(int x, int y, int width, int height) : QGraphicsScene(x, y, width, height) {}

    ~CustomGraphicsScene() = default;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CUSTOMGRAPHICSSCENE_H
