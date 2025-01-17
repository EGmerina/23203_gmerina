#ifndef CIRCLE_H
#define CIRCLE_H

#include <QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QMouseEvent>


class Circle : public QGraphicsEllipseItem {
public:
    Circle(const qreal x, const qreal y, const qreal r, QGraphicsScene *scene);
    virtual ~Circle(){}

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

};


#endif // CIRCLE_H
