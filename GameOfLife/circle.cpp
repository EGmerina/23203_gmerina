#include "circle.h"


    Circle::Circle(const qreal x, const qreal y, const qreal r, QGraphicsScene *scene) :
        QGraphicsEllipseItem(x, y, r, r)
    {
        setPen( QPen(QBrush(Qt::black), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin) );
        setRect(x, y, r, r);
        setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
        scene->addItem(this);
    }


    void Circle::mousePressEvent(QMouseEvent *event) {

        QMessageBox msgBox;
        QString QResult{ QString::fromLatin1("Circle clicked") };
        msgBox.setText( QResult );
        msgBox.exec();
    }

