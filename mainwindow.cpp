#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    // Enable antialiasing for smoother graphics
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    //get max size of port and give it to scene handler class
    Map = std::make_shared<GMap>(16,32);
    Ghandler = std::make_shared<GraphicsSceneHandler>(scene
                                ,ui->graphicsView->maximumViewportSize().width()
                                ,ui->graphicsView->maximumViewportSize().height()
                                ,Map
                                ,this);

    intialize_line_edits();

    scriptLoader_ = std::make_shared<scriptLoader>(Map);

    connect(Ghandler->Map.get(), &GMap::changeStartSquareText, this, &MainWindow::handleStartSquareText);
    connect(Ghandler->Map.get(), &GMap::changeEndSquareText, this, &MainWindow::handleEndSquareText);

}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::intialize_line_edits(){
    Gsize_line_edit = ui->Gsize_line_edit;
    Gsize_line_edit->setPlaceholderText("enter map size");
    ui->Map_file_name_line_edit->setPlaceholderText("New Map");
    ui->Ssize_line_edit->setPlaceholderText("enter square size");


}

void MainWindow::on_load_map_clicked()
{

    QString filename = QFileDialog::getOpenFileName(nullptr,
                                                    "Open CSV File",
                                                    "",
                                                    "CSV Files (*.csv);;All Files (*)");
    if (filename.isEmpty()) return; // If the user cancels, do nothing

    Ghandler->Map->loadFromCSV(filename);
    Ghandler->draw_map();

    // get the filename from the file path and display it on the line text edit
    ui->Map_file_name_line_edit->setPlaceholderText(filename.mid(filename.lastIndexOf('/') + 1));
    ui->Gsize_line_edit->setText(QString::number(Ghandler->Map->getGridSize()));
    ui->Ssize_line_edit->setText(QString::number(Ghandler->Map->getSqaureSize()));
    handleStartSquareText(0,0);
    handleEndSquareText(0,0);

}


void MainWindow::on_export_map_clicked()
{
    Ghandler->Map->saveToCSV();
}



void MainWindow::on_Update_Map_button_clicked()
{
    int Gsize,Ssize;

    // if there are grid size or square size changes then we resize;
    if (!ui->Ssize_line_edit->text().isEmpty()){
        Ssize =  ui->Ssize_line_edit->text().toInt();
    } else{
        Ssize = Ghandler->Map->getSqaureSize();
    }
    if (!ui->Gsize_line_edit->text().isEmpty()){
        Gsize = ui->Gsize_line_edit->text().toInt();
    } else {
        Gsize = Ghandler->Map->getGridSize();
    }

    Ghandler->Map->resize(Gsize , Ssize);
    Ghandler->draw_map();
    //modify text boxes
    ui->Gsize_line_edit->setText(QString::number(Ghandler->Map->getGridSize()));
    ui->Ssize_line_edit->setText(QString::number(Ghandler->Map->getSqaureSize()));
}

void MainWindow::on_update_square_size_button_clicked()
{
    static int Ssize=15;
    // if there are square size changes then we resize;
    if (!ui->Ssize_line_edit->text().isEmpty()){
        Ssize =  ui->Ssize_line_edit->text().toInt();
    } else{
        Ssize = Ghandler->Map->getSqaureSize();
    }
    Ghandler->Map->setSquareSize(Ssize);

}


void MainWindow::on_actionexit_triggered()
{
    QApplication::quit();
}



// Map control buttons
void MainWindow::on_set_start_square_clicked()
{
    Ghandler->Map->change_type(5);

}
void MainWindow::on_set_end_square_clicked()
{
    Ghandler->Map->change_type(6);
}
void MainWindow::on_set_obstacle_square_clicked()
{
    Ghandler->Map->change_type(4);
}
void MainWindow::on_set_empty_sqaure_clicked()
{
    Ghandler->Map->change_type(1);
}

void MainWindow::on_empty_all_squares_clicked()
{
    int Gsize = Ghandler->Map->getGridSize();
    Ghandler->Map->change_type(1);
    for(int i=0; i < Gsize ;i++){
        for(int j=0; j < Gsize ;j++){
            if (Ghandler->Map->Grid[i][j] != nullptr) {
                Ghandler->Map->Grid[i][j]->type=Global_button_square_type;
                Ghandler->Map->Grid[i][j]->update_color();
                Ghandler->Map->Grid[i][j]->setPen(QPen(Qt::black));
                Ghandler->Map->type_grid[i][j] = Global_button_square_type;
            }
        }
    }
}


void MainWindow::on_gray_all_squares_clicked()
{
    int Gsize = Ghandler->Map->getGridSize();
    Ghandler->Map->change_type(0);
    for(int i=0; i < Gsize ;i++){
        for(int j=0; j < Gsize ;j++){
            if (Ghandler->Map->Grid[i][j] != nullptr) {
                Ghandler->Map->Grid[i][j]->type=Global_button_square_type;
                Ghandler->Map->Grid[i][j]->update_color();
                Ghandler->Map->Grid[i][j]->setPen(QPen(Qt::black));
                Ghandler->Map->type_grid[i][j] = Global_button_square_type;
            }
        }
    }

}


void MainWindow::on_Zoom_in_clicked()
{
    if (!ui->graphicsView) return;
    Ghandler->Zoom(ui->graphicsView,true);

}


void MainWindow::on_Zoom_out_clicked()
{
    if (!ui->graphicsView) return;
    Ghandler->Zoom(ui->graphicsView,false);
}

void MainWindow::on_Zoom_Reset_clicked()
{
    if (!ui->graphicsView) return;
    ui->graphicsView->resetTransform();
}



void MainWindow::on_actionLoad_map_triggered()
{
    MainWindow::on_load_map_clicked();
}



void MainWindow::on_actionexport_map_triggered()
{
    MainWindow::on_export_map_clicked();
}




void MainWindow::on_Algorithm_Load_clicked()
{
    int choice  = ui->Algorithm_dropdown->currentIndex();

    if(scriptLoader_ ->loadAlgorithm(choice)){
        QString algName = scriptLoader_ ->getAlgName();
        ui->Algorithm_Name_linedit->setText(algName);
    }

}


void MainWindow::on_pushButton_clicked()
{
    static bool start_pause = true;
    if(start_pause){
        scriptLoader_->startAlgorithm();
    }else{
        scriptLoader_->pauseAlgorithm();
    }
    start_pause = !start_pause;
}

void MainWindow::on_clear_clicked()
{
   scriptLoader_->stopAlgorithm();
}
void MainWindow::handleStartSquareText(int x, int y){
    ui->start_square_line_edit->setText(Ghandler->Map->get_start_square_txt());

}
void MainWindow::handleEndSquareText(int x, int y){
    ui->end_square_line_edit->setText(Ghandler->Map->get_end_square_txt());
}
