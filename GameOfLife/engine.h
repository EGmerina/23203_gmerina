#ifndef ENGINE_H
#define ENGINE_H
#include <QtWidgets/QGraphicsScene>
#include <QObject>

#include "space.h"

class Engine : public QObject{
public:

    static Engine* getInstance(){
        static Engine e;
        return &e;
    }

    void update(Space * space, QGraphicsScene * scene);

    void start(Space * space, QGraphicsScene * scene);

    void stop();

    void show(Space * space, QGraphicsScene * scene);

    void reload(Space * space, QGraphicsScene * scene);

    bool isEngineStopped();

private:
    bool stopflag = true;

};

#endif // ENGINE_H
