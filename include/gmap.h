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
    const int getSquareSize();
    //void setGidSize(int newSize);
    void setSquareSize(int newSize);

    //method for python
    std::array<int, 2>  getStartSquare() const;
    std::array<int, 2>  getEndSquare() const;

    //method for ui text labels
    QString get_start_square_txt() const;
    QString get_end_square_txt() const;

    // map backup
    void backup_map();
    void reset_map();

signals:
    void changeStartSquareText(int x, int y);
    void changeEndSquareText(int x, int y);
private slots:
    void handleClickedSquare(int x, int y);

private:

    QVector<QVector<int>>  backupMap;
    // num_of_squares in a row or col;
    int num_of_squares = 16;
    int square_size = 30;
    square* square_;
    square* start_square = nullptr;
    square* end_square = nullptr;
    void if_unique(int x,int y);

};





#endif // GMAP_H
