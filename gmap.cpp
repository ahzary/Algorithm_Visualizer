#include "gmap.h"

GMap::GMap(int num_of_squares,int square_size){

    //intialize 2 2d vectors for the map

    this->num_of_squares = num_of_squares;
    this->square_size = square_size;

    Grid.resize(num_of_squares);
    type_grid.resize(num_of_squares);

    for(int i=0;i<num_of_squares;i++){

        Grid[i].resize(num_of_squares);
        type_grid[i].resize(num_of_squares);

        for(int j=0;j<num_of_squares;j++){

            //create a new square and fill in the paraneters
            square = new QGraphicsRectItem();
            square->setRect(i * square_size,j * square_size,square_size,square_size);
            Grid[i][j] = square;  // Add the square to Grid
            type_grid[i][j] = 0;   // Initialize the grid type to 0

        }
    };
}

void GMap::update_map(){
// go over each grid square and update its color

        for(int i=0;i<num_of_squares;i++){
            for(int j=0;j<num_of_squares;j++){
                if (Grid[i][j] != nullptr) {
                    Grid[i][j]->setBrush(ColorCode(type_grid[i][j]));
                    Grid[i][j]->setPen(QPen(Qt::black));
                }
            }
        }

    }


 QBrush GMap::ColorCode(int i){
  // convert type_grid to color_grid (Grid)

        switch (i) {
        case 0: return QBrush(Qt::gray); //unassighned sapce
        case 1: return QBrush(Qt::white);//clear space
        case 2: return QBrush(Qt::blue); //cleared space
        case 3: return QBrush(Qt::red);  //current step
        case 4: return QBrush(Qt::black);//obstacle
        case 5: return QBrush(Qt::green);//path to target
        default:return QBrush(Qt::white);

        }

    }

 GMap::~GMap() {
     for (int i = 0; i < num_of_squares; i++) {
         for (int j = 0; j < num_of_squares; j++) {
             delete Grid[i][j];
         }
     }
 }

