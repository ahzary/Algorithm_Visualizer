#ifndef GMAP_H
#define GMAP_H
#define default_size 16
#include <QGraphicsRectItem>
#include <QVector>
#include <QBrush>
#include <QPen>

class GMap : public QObject
{
    Q_OBJECT
public:
    GMap(int num_of_squares,int square_size);
    ~GMap();
    QVector<QVector<QGraphicsRectItem*>> Grid;
    void update_map();
    QBrush ColorCode(int i);
    QVector<QVector<int>>  type_grid;


private:

    // num_of_squares in a row or col;
    int num_of_squares,square_size;
    QGraphicsRectItem* square;

};





#endif // GMAP_H
