#ifndef CIRCLE_H
#define CIRCLE_H

#include <QApplication>
#include <QMessageBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsEllipseItem>

class Circle : public QGraphicsEllipseItem {
public:
    Circle(const qreal x, const qreal y, const qreal r, QGraphicsScene *scene);
    virtual ~Circle(){}

    void mousePressEvent(QMouseEvent *event);
};


#endif // CIRCLE_H
