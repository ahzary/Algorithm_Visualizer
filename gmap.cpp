#include "gmap.h"
//#include <QDebug>

GMap::GMap(int num_of_squares,int square_size){

    //intialize 2 2d vectors for the map

    this->num_of_squares = num_of_squares;
    this->square_size = square_size;
    Grid.empty();
    type_grid.empty();
    Grid.resize(num_of_squares);
    type_grid.resize(num_of_squares);

    for(int i=0;i<num_of_squares;i++){

        Grid[i].resize(num_of_squares);
        type_grid[i].resize(num_of_squares);

        for(int j=0;j<num_of_squares;j++){

            //create a new square and fill in the paraneters
            square_ = new square();
            //assign position,size,type and grid id (x,y) to the square
            square_->setRect(i * square_size,j * square_size,square_size,square_size);
            square_->type=0;
            square_->x=i;
            square_->y=j;
            //fill out the vectors with the squares
            Grid[i][j] = square_;  // Add the square to Grid
            type_grid[i][j] = square_->type;// fill out the vector with types


            // Connect the square's clicked signal to GMap's slot
            connect(Grid[i][j], &square::clicked, this, &GMap::handleClickedSquare);

        }
    };
}

void GMap::update_map(){
// go over each grid square and update its color

        for(int i=0;i<num_of_squares;i++){
            for(int j=0;j<num_of_squares;j++){
                if (Grid[i][j] != nullptr) {
                    Grid[i][j]->update_color();
                    Grid[i][j]->setPen(QPen(Qt::black));
                    Grid[i][j]->setScale(size_multiplier);
                }
            }
        }

    }

 GMap::~GMap() {
     for (int i = 0; i < num_of_squares; i++) {
         for (int j = 0; j < num_of_squares; j++) {
             delete Grid[i][j];
         }
     }
 }

 void GMap::change_type(int new_type_){
     Global_button_square_type = new_type_;
     /*switch (new_type_) {
     case 3: start_squares->type = 1; start_squares->update_color(); break;
     case 4: end_squares->type = 1; end_squares->update_color(); break;
     default: break;
     }*/

 }

 void GMap::saveToCSV(){
    QString filename = QFileDialog::getSaveFileName(nullptr,
                                                     "CSV Files (*.csv);;All Files (*)");

    if (filename.isEmpty()) return; // If the user cancels, do nothing
    filename += ".csv";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& row : this->type_grid) {
            for (int i = 0; i < row.size(); i++) {
                out << row[i];
                if (i < row.size() - 1) out << ",";  // Add comma between values
            }
            out << "\n";  // New line for the next row
        }
        file.close();
    }

 }

 void GMap::loadFromCSV(const QString& filename) {


     QFile file(filename);
     if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         QTextStream in(&file);
         QVector<QVector<int>> newTypeGrid;

         int rowIdx = 0;
         while (!in.atEnd()) {
             QString line = in.readLine();
             QStringList values = line.split(",");

             QVector<int> row;
             for (const QString& val : values) {
                 row.append(val.toInt());
             }

             newTypeGrid.append(row);
             rowIdx++;
         }

         file.close();

         // Ensure correct dimensions
         if (newTypeGrid.size() == num_of_squares && newTypeGrid[0].size() == num_of_squares) {
             type_grid = newTypeGrid;

             // Apply new types to the grid squares
             for (int i = 0; i < num_of_squares; i++) {
                 for (int j = 0; j < num_of_squares; j++) {
                     if (Grid[i][j] != nullptr) {
                         Grid[i][j]->type = type_grid[i][j];
                         Grid[i][j]->update_color();
                     }
                 }
             }
         }
     }
 }

 void GMap::handleClickedSquare(int x, int y){

    //qDebug() << "New handleClickedSquare square set at:" << x << y;
    Grid[x][y]->type = Global_button_square_type;
    type_grid[x][y] = Grid[x][y]->type;
    if_unique(x,y);
    Grid[x][y]->update_color();


 }

 void GMap::if_unique(int x,int y){
    //qDebug() << "if_unique square set at:" << x << y << Global_button_square_type;
     switch (Global_button_square_type) {
     case 5:
         if(start_square != nullptr){
             start_square->type = 1;
             start_square->update_color();
         }
            // qDebug() << "New start square set at:" << x << y;
            start_square = Grid[x][y];
         break;

     case 6:
         if(end_square != nullptr){
             end_square->type = 1;
             end_square->update_color();
         }
            end_square = Grid[x][y];
         break;
     default: break;
     }

 }


