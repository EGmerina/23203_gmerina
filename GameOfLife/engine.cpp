#include "engine.h"
#include <QtWidgets/QGraphicsScene>
#include <QTimer>


void Engine::show(Space *space, QGraphicsScene *scene, DrawnSpace *drawnspace)
{
    space->updateSpace();
    drawnspace->drawSpace(space, scene);
}

void Engine::start(Space *space, QGraphicsScene *scene, DrawnSpace *drawnspace)
{
    if (!stopflag)
        return;
    stopflag = false;
    update(space, scene, drawnspace);
}

void Engine::update(Space *space, QGraphicsScene *scene, DrawnSpace *drawnspace)
{
    show(space, scene, drawnspace);
    if (!stopflag)
    {
        QTimer::singleShot(speed, Qt::PreciseTimer, this, [space, scene, drawnspace, this]()
                           { Engine ::update(space, scene, drawnspace); });
    }
}

void Engine::stop()
{
    stopflag = true;
}

void Engine::reload(Space *space, QGraphicsScene *scene, DrawnSpace *drawnspace)
{
    space->reload();
    drawnspace->drawSpace(space, scene);
    stopflag = true;
}

bool Engine::isEngineStopped()
{
    return stopflag;
}

void Engine::setSpeed(size_t newspeed){
    this->speed = newspeed;
}
