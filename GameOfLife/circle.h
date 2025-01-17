#ifndef CIRCLE_H
#define CIRCLE_H

#include <QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QWidget>


class Circle : public QGraphicsEllipseItem {
public:
    Circle(const qreal x, const qreal y, const qreal r, QGraphicsScene *scene);
     ~Circle(){}
protected:
    void mousePressEvent(QMouseEvent *event) ;


};


#endif // CIRCLE_H
