#ifndef GRAPHICSSCENEHANDLER_H
#define GRAPHICSSCENEHANDLER_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QObject>
#include <memory>
#include <QBrush>
#include "gmap.h"


class GraphicsSceneHandler : public QObject
{
    Q_OBJECT
public:
    GraphicsSceneHandler(QGraphicsScene* Scene
                         ,int intial_background_size_W_,int intial_background_size_H_
                         ,std::shared_ptr<GMap> Map_
                         , QObject *parent = nullptr);


    std::shared_ptr<GMap> Map ;

    void addrect();
    void clearScene();
    //void resize_map(int newSize);
    void Zoom(QGraphicsView* view,bool in);
    int intial_background_size_W,intial_background_size_H;
    //int Gsize = 16;
    //int Ssize = 30;


    void draw_map();
    void update();

    QGraphicsRectItem* background;

private:
    QGraphicsScene *scene;


};

#endif // GRAPHICSSCENEHANDLER_H
