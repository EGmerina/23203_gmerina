#include "circle.h"


    Circle::Circle(const qreal ax, const qreal ay, const qreal wx, const qreal wy, QGraphicsScene *scene) :
        QGraphicsEllipseItem(ax, ay, wx, wy)
    {
        setPen( QPen(QBrush(Qt::black), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin) );
        setBrush( QBrush(Qt::gray) );
        setRect(ax, ay, wx, wy);
        setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
        scene->addItem(this);
    }


    void Circle::mousePressEvent(QMouseEvent *event) {

        QMessageBox msgBox;
        QString QResult{ QString::fromLatin1("Circle clicked") };
        msgBox.setText( QResult );
        msgBox.exec();
    }

