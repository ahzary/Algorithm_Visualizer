#include "graphicsscenehandler.h"


GraphicsSceneHandler::GraphicsSceneHandler(QGraphicsScene* Scene
                                           ,int intial_background_size_W_
                                           ,int intial_background_size_H_
                                           , QObject *parent)
                                        : scene(Scene),QObject(parent)
                                        ,intial_background_size_W(intial_background_size_W_)
                                        ,intial_background_size_H(intial_background_size_H_){

    background = scene->addRect(0, 0, intial_background_size_W, intial_background_size_H, QPen(Qt::black), QBrush(Qt::white));

    draw_map(Gsize,Ssize);
    Map->update_map();
}


void GraphicsSceneHandler::addrect(){
    static int  i=1;
    Map->type_grid[1][1] = i;
    Map->update_map();
    i++;
    if (i==4 ) {i=0;}

}
void GraphicsSceneHandler::draw_map(int Gsize,int Ssize){

    for(int i=0;i<Gsize;i++){
        for(int j=0;j<Gsize;j++){
            if (Map->Grid[i][j] != nullptr) {
                scene->addItem(Map->Grid[i][j]);
            }
        }
    };


}


void GraphicsSceneHandler::clearScene(){
    scene->clear();
}


void GraphicsSceneHandler::resize_map(int newsize){
    Map->Grid.resize(newsize);
    for(int i=0;i<newsize;i++){
            Map->Grid[i].resize(newsize);

    };
    //Map->draw_map();
    Map->update_map();

}
