#ifndef GMAP_H
#define GMAP_H

#include "square.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileDialog>


class GMap : public QObject
{
    Q_OBJECT
public:
    GMap(int num_of_squares,int square_size);
    ~GMap();

    float size_multiplier = 1.0;

    QVector<QVector<square*>> Grid;
    QVector<QVector<int>>  type_grid;

    void update_map();
    void change_type(int new_type_);
    void saveToCSV();
    void loadFromCSV(const QString& filename);
    void resize(int newsize,int squareSize);
    int* num_of_squares_ptr = &num_of_squares;

    const int getGridSize();
    const int getSqaureSize();
    void setGidSize(int newSize);
    void setSquareSize(int newSize);

private slots:
    void handleClickedSquare(int x, int y);

private:

    // num_of_squares in a row or col;
    int num_of_squares = 16;
    int square_size = 30;
    square* square_;
    square* start_square = nullptr;;
    square* end_square = nullptr;;
    void if_unique(int x,int y);

};





#endif // GMAP_H
