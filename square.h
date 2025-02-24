#ifndef SQUARE_H
#define SQUARE_H
#include <QGraphicsRectItem>
#include <QVector>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QPair>
#include <QObject>

QBrush ColorCode(int i);
extern int Global_button_square_type;


class square : public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit square(QObject* parent = nullptr);
    //square();
    int type=0;
    int x,y;
    void update_color();
signals:
    void clicked(int x,int y);  // Signal to emit when clicked

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    //void buttonDownPos(QGraphicsSceneMouseEvent* event) override;  // drag function
};

#endif // SQUARE_H
