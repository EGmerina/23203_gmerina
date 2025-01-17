#include "engine.h"
#include <QtWidgets/QGraphicsScene>
#include "space.h"
#include "drawnspace.h"
#include <QTimer>


#define DELAY 1000


void Engine:: start(Space * space, QGraphicsScene * scene){
    stopflag = false;
    update(space,  scene);
}

void Engine:: update(Space * space, QGraphicsScene * scene){
    space->updateSpace();
    DrawnSpace::drawSpace(space, scene);
   if(!stopflag){
        QTimer::singleShot(DELAY,Qt::PreciseTimer,this, [space, scene, this](){Engine :: update(space, scene);}); //TODO not singleshot
    }
}

void Engine:: stop(){
    stopflag = true;
}

