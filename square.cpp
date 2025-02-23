#include "square.h"
int Global_button_square_type = 0;



QBrush ColorCode(int type){
    // convert type_grid to color_grid (Grid)

    // add multiple types of obsatcles
    switch (type) {
    case 0: return QBrush(Qt::gray); //unassighned sapce
    case 1: return QBrush(Qt::white);//clear space
    case 2: return QBrush(Qt::blue); //cleared space
    case 3: return QBrush(Qt::red);  //current step
    case 4: return QBrush(Qt::black);//obstacle
    case 5: return QBrush(Qt::green);//start square
    case 6: return QBrush(Qt::darkMagenta);//end square
    default:return QBrush(Qt::white);

    }

}

// square functions
square::square(QObject* parent) : QObject(parent){
    setAcceptHoverEvents(true);
}
void square::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    //setBrush(QBrush(Qt::red));
    setPen(QPen(Qt::red));  // Change color on hover
    update();
    QGraphicsRectItem::hoverEnterEvent(event);
}

void square::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setPen(QPen(Qt::black));  // Reset color when not hovering
    update();
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void square::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(x,y);  // Emit the clicked signal
    }

    QGraphicsRectItem::mousePressEvent(event);  // Call base class method
}
void square::update_color(){


    this->setBrush(ColorCode(this->type));

}


