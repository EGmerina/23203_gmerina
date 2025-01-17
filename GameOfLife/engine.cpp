#include "engine.h"
#include <QtWidgets/QGraphicsScene>
#include "space.h"
#include "drawnspace.h"
#include <unistd.h>


void Engine:: start(Space * space, QGraphicsScene * scene){
    stopflag = false;
    update(space,  scene);
}

void Engine:: update(Space * space, QGraphicsScene * scene){
    space->updateSpace();
    DrawnSpace::drawSpace(space, scene);
    usleep(2000);
    if(!stopflag){
         update(space, scene);
    }
}

void Engine:: stop(){
    stopflag = true;
}

