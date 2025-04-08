#include "gmap.h"
#include <QDebug>

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
qDebug() << "entering update map" ;
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
 }

 void GMap::saveToCSV(){
    QString filename = QFileDialog::getSaveFileName(nullptr,
                                                     "CSV Files (*.csv);;All Files (*)");

    // If the user cancels, do nothing
    if (filename.isEmpty()) return;
    // Ensure the filename has the .csv extension
    if (!filename.endsWith(".csv", Qt::CaseInsensitive)) {
        filename += ".csv";
    }
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

     qDebug() << "file loading started" ;
     QFile file(filename);
     if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         QTextStream in(&file);
         QVector<QVector<int>> newTypeGrid;

         //int rowIdx = 0;
         while (!in.atEnd()) {
             QString line = in.readLine();
             QStringList values = line.split(",");

             QVector<int> row;
             for (const QString& val : values) {
                 row.append(val.toInt());
             }

             newTypeGrid.append(row);
             //rowIdx++;
         }

         file.close();

         // Ensure correct dimensions
            resize(newTypeGrid.size(),this->square_size);
            type_grid = newTypeGrid;
             // Apply new types to the grid squares
         for (int i = 0; i < num_of_squares; i++) {
             for (int j = 0; j < num_of_squares; j++) {
                 if (Grid[i][j] != nullptr) {
                     // populate start and end square pointers
                     if (type_grid[i][j] == 5){start_square = Grid[i][j];}
                     else if (type_grid[i][j] == 6){end_square = Grid[i][j];}
                     // populate both vectors
                     Grid[i][j]->type = type_grid[i][j];
                     Grid[i][j]->update_color();
                 } // if end
             } // inner for end
         } // outer for end


     } // end of if (file.open(QIODevice::ReadOnly | QIODevice::Text))
 } // end of GMap::loadFromCSV(const QString& filename)

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
     case 5: // start square type
         if(start_square != nullptr){
             //reset the previous start square
             start_square->type = 1;
             start_square->update_color();
         }
            // qDebug() << "New start square set at:" << x << y;
            start_square = Grid[x][y];
         emit changeStartSquareText(x,y);
         break;

     case 6: // end square type
         if(end_square != nullptr){
             //reset the previous end square
             end_square->type = 1;
             end_square->update_color();
         }
            end_square = Grid[x][y];
            emit changeEndSquareText(x,y);
         break;
     default:
         break;
    }

 }

 void GMap::resize(int newSize,int square_size){
    qDebug() << "entered resize" <<newSize << square_size ;
    for (int i = 0; i < num_of_squares; i++) {
        for (int j = 0; j < num_of_squares; j++) {
            delete Grid[i][j];  // delete old square and free memory to prevent leaks
            Grid[i][j] = nullptr;
        }
    }
    Grid.clear();
    type_grid.clear();

    num_of_squares = newSize;
    this->square_size = square_size;

    Grid.resize(num_of_squares, QVector<square*>(num_of_squares, nullptr));
    type_grid.resize(num_of_squares, QVector<int>(num_of_squares, 0));

    for (int i = 0; i < num_of_squares; i++) {
        for (int j = 0; j < num_of_squares; j++) {
            Grid[i][j] = new square();
            Grid[i][j]->type=0;
            Grid[i][j]->x=i;
            Grid[i][j]->y=j;
            Grid[i][j]->setRect(i * square_size, j * square_size, square_size, square_size);
            connect(Grid[i][j], &square::clicked, this, &GMap::handleClickedSquare);
        }
    }
    update_map();
 }
 const int GMap::getGridSize(){
     return num_of_squares;
 }
 const int GMap::getSqaureSize(){
     return square_size;
 }
 void GMap::setSquareSize(int newSize){
     for (int i = 0; i < num_of_squares; i++) {
         for (int j = 0; j < num_of_squares; j++) {
             Grid[i][j]->setRect(i * newSize, j * newSize, newSize, newSize);
         }}

 }
std::array<int, 2>  GMap::getStartSquare()const {

    if(start_square == nullptr){
        qDebug()<< "start square is null" ;
        return {0,0};
    }

    return {start_square->x,start_square->y};
 }
std::array<int, 2>  GMap::getEndSquare()const {
    if(end_square == nullptr){
        qDebug()<< "end square is null" ;
        return {0,0};
    }
    return {end_square->x,end_square->y};
 }
QString GMap::get_start_square_txt()const{
    QString result = "x: " + QString::number(start_square->x) + ", " + "y: " + QString::number(start_square->y);
    return result;
}
QString GMap::get_end_square_txt()const{
    QString result = "x: " + QString::number(end_square->x) + ", " + "y: " + QString::number(end_square->y);
    return result;
}
void GMap::backup_map(){
    backupMap = type_grid;
}
void GMap::reset_map(){
    type_grid = backupMap;
    for (int i = 0; i < num_of_squares; i++) {
        for (int j = 0; j < num_of_squares; j++) {
            if (type_grid[i][j] == 5){start_square = Grid[i][j];}
            else if (type_grid[i][j] == 6){end_square = Grid[i][j];}
            Grid[i][j]->type = type_grid[i][j];
            Grid[i][j]->update_color();

        }
        }
}
