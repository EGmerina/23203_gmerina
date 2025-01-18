#include "drawnspace.h"



void DrawnSpace:: drawSpace(Space * space, QGraphicsScene* scene){

    this->drawnspace.clear();
    size_t distbtwcells = (scene->width()/space->getSize())*0.9;
    size_t r = distbtwcells*0.8;
    for(size_t y=0; y< space->getSize(); ++y){
        for(size_t x=0; x< space -> getSize(); ++x){
            std:: unique_ptr<Circle> c = std::make_unique<Circle>(distbtwcells*y + distbtwcells/2,  distbtwcells*x + distbtwcells/2,  r, scene);
            c->setSpace(space);
            c->setPosition(x, y);
            if(space->isCellAlive(x, y)){
                c->setBrush( QBrush(Qt::red) );
            }else{

                c->setBrush( QBrush(Qt::gray) );
            }
            this->drawnspace.push_back(std::move(c));
        }
    }
}
