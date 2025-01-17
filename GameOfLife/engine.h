#ifndef ENGINE_H
#define ENGINE_H
#include <QtWidgets/QGraphicsScene>
#include "space.h"

class Engine{
public:
    void update(Space * space, QGraphicsScene * scene);
    void start(Space * space, QGraphicsScene * scene);
    void stop();
private:
    bool stopflag = false;
};

#endif // ENGINE_H
