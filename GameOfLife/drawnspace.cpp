#include "drawnspace.h"
#include "circle.h"


void DrawnSpace:: drawSpace(Space * space, QGraphicsScene* scene){
    size_t distbtwcells = (scene->width()/space->getSize())*0.9;
    size_t r = distbtwcells*0.8;
    for(size_t i=0; i< space->getSize(); ++i){
        for(size_t j=0; j< space -> getSize(); ++j){
            Circle* c = new Circle(  distbtwcells*i + distbtwcells/2,  distbtwcells*j + distbtwcells/2,  r, scene);
            c->setSpace(space);
            c->setPosition(i, j);
            if(space->isCellAlive(i, j)){
                c->setBrush( QBrush(Qt::red) );
            }else{

                c->setBrush( QBrush(Qt::gray) );
            }
        }
    }
}
