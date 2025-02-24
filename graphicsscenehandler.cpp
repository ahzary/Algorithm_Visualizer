#include "graphicsscenehandler.h"


GraphicsSceneHandler::GraphicsSceneHandler(QGraphicsScene* Scene
                                           ,int intial_background_size_W_
                                           ,int intial_background_size_H_
                                           , QObject *parent)
                                        : scene(Scene),QObject(parent)
                                        ,intial_background_size_W(intial_background_size_W_)
                                        ,intial_background_size_H(intial_background_size_H_)
{

    //background = scene->addRect(0, 0, 1, 1, QPen(Qt::black), QBrush(Qt::white));
    //Ssize = intial_background_size_W_ / Gsize;
    Map = std::make_shared<GMap>(16,30);
    draw_map();
    Map->update_map();
}


void GraphicsSceneHandler::addrect(){
    static int  i=1;
    Map->type_grid[1][1] = i;
    Map->update_map();
    i++;
    if (i==4 ) {i=0;}

}
void GraphicsSceneHandler::clearScene(){
    //scene->clear();
}

void GraphicsSceneHandler::draw_map(){

    int Gsize = Map->getGridSize();
    for (QGraphicsItem* item : scene->items()) {
        if (dynamic_cast<square*>(item) != nullptr) {
            scene->removeItem(item);
            delete item;
        }
    }

    for(int i=0;i<Gsize;i++){
        for(int j=0;j<Gsize;j++){
            if (Map->Grid[i][j] != nullptr) {
                scene->addItem(Map->Grid[i][j]);
            }
        }
    };
}
void GraphicsSceneHandler::Zoom(QGraphicsView* view,bool in){
    float scaleFactor = 1.0;
    if (in){
        scaleFactor += 0.1;
    }else {
        scaleFactor -= 0.1;
    }
        view->scale(scaleFactor, scaleFactor);


}
