#include "circle.h"
#include <iostream>


    Circle::Circle(const qreal x, const qreal y, const qreal r, QGraphicsScene *scene) :
        QGraphicsEllipseItem(x, y, r, r)
    {
        setPen( QPen(QBrush(Qt::black), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin) );
        setRect(x, y, r, r);
        setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
        scene->addItem(this);
    }


    void Circle::mousePressEvent(QMouseEvent *event) {
        if(event->button()== Qt::MouseButton:: LeftButton){
            std::cout <<"***"<<std::endl;
            this->setBrush( QBrush(Qt::red) );
        }


    }

