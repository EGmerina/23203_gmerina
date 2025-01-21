#ifndef CIRCLE_H
#define CIRCLE_H

#include <QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QWidget>

#include "space.h"


class Circle : public QGraphicsEllipseItem {
public:
    Circle()= delete;
    Circle(const qreal x, const qreal y, const qreal r, QGraphicsScene *scene );
    ~Circle()= default;

    void setPosition(size_t x , size_t y );

    void setSpace(Space * space);

    void repaintLive();

    void repaintDead();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;

private:
    size_t x = 0;
    size_t y = 0;
    Space * space;

    void repaint();
};


#endif // CIRCLE_H
