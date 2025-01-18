#include "engine.h"
#include <QtWidgets/QGraphicsScene>
#include <QTimer>


#define DELAY 700

void Engine::show(Space * space, QGraphicsScene * scene, DrawnSpace * drawnspace){
    space->updateSpace();
    drawnspace->drawSpace(space, scene);
}

void Engine:: start(Space * space, QGraphicsScene * scene, DrawnSpace * drawnspace){
    if(!stopflag) return;
    stopflag = false;
    update(space,  scene, drawnspace);
}

// update = [space, scene, drawnspace, this](){
//     space->updateSpace();
//     drawnspace->drawSpace(space, scene);
// }

// void Engine::update(std::function<void, void> update){
//     update();
//     if(!stopflag){
//         QTimer::singleShot(DELAY,Qt::PreciseTimer,this, [](){Engine :: update(update);}); //TODO not singleshot
//     }
// }

void Engine:: update(Space * space, QGraphicsScene * scene, DrawnSpace * drawnspace){
    show(space, scene, drawnspace);
    if(!stopflag){
        QTimer::singleShot(DELAY,Qt::PreciseTimer,this, [space, scene, drawnspace, this](){Engine :: update(space, scene, drawnspace);}); //TODO not singleshot
    }
}

void Engine:: stop(){
    stopflag = true;
}

void Engine:: reload(Space * space, QGraphicsScene * scene, DrawnSpace * drawnspace){
    space->reload();
    drawnspace->drawSpace(space, scene);
    stopflag = true;
}

bool Engine::isEngineStopped(){
    return stopflag;
}
