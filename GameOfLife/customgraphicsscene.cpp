#include "customgraphicsscene.h"

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->items(event->scenePos());
    if (items.size() > 0 && event->buttons() == Qt::LeftButton)
    {
        static_cast<Circle *>(items[0])->repaintLive();
    }
    if (items.size() > 0 && event->buttons() == Qt::RightButton)
    {
        static_cast<Circle *>(items[0])->repaintDead();
    }
    QGraphicsScene::mouseMoveEvent(event);
}
