#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QtWidgets/QGraphicsScene>

#include "drawnspace.h"
#include "space.h"


class Engine : public QObject{
public:

    static Engine* getInstance(){
        static Engine e;
        return &e;
    }

    void update(Space * space, QGraphicsScene * scene, DrawnSpace * drawnspace);

    void start(Space * space, QGraphicsScene * scene, DrawnSpace * drawnspace);

    void stop();

    void show(Space * space, QGraphicsScene * scene, DrawnSpace * drawnspace);

    void reload(Space * space, QGraphicsScene * scene, DrawnSpace * drawnspace);

    bool isEngineStopped();

private:
    bool stopflag = true;

};

#endif // ENGINE_H
