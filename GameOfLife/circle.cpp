#include "circle.h"
#include "engine.h"


    Circle::Circle(const qreal x, const qreal y, const qreal r, QGraphicsScene *scene) :
    QGraphicsEllipseItem(x, y, r, r)
    {
        setPen( QPen(QBrush(Qt::black), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin) );
        setRect(x, y, r, r);
        setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
        scene->addItem(this);
    }

    void Circle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        if(Engine:: getInstance()->isEngineStopped()){
            repaint();
        }
    }

    void  Circle::setPosition(size_t x , size_t y ){
        this->x = x;
        this->y = y;
    }

    void Circle::setSpace(Space * space){
        this->space = space;
    }


    void Circle:: repaint(){
        if(space->isCellAlive(x, y)){
            space->removeCell(x, y);
            this->setBrush(Qt:: gray);
        }else{
            space->addCell(x, y);
            this->setBrush(Qt:: red);
        }
    }
