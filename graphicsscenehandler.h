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
                         , QObject *parent = nullptr);



    void addrect();
    void clearScene();
    void resize_map(int newsize);

    int intial_background_size_W,intial_background_size_H;
    int Gsize =16;
    int Ssize =intial_background_size_W/Gsize;

    std::shared_ptr<GMap> Map = std::make_shared<GMap>(Gsize,Ssize);
    void draw_map(int num_of_squares_,int square_size_);


    QGraphicsRectItem* background;

private:
    QGraphicsScene *scene;


};

#endif // GRAPHICSSCENEHANDLER_H
