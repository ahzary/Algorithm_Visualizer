#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

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

    Ghandler = std::make_shared<GraphicsSceneHandler>(scene
                                ,ui->graphicsView->maximumViewportSize().width()
                                ,ui->graphicsView->maximumViewportSize().height()
                                ,this);

    Gsize_line_edit = ui->Gsize_line_edit;
    Gsize_line_edit->setPlaceholderText("enter map size");
    ui->Map_file_name_line_edit->setPlaceholderText("Map file name");

}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_load_map_clicked()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,
                                                    "Open CSV File",
                                                    "",
                                                    "CSV Files (*.csv);;All Files (*)");
    if (filename.isEmpty()) return; // If the user cancels, do nothing
    Ghandler->Map->loadFromCSV(filename);
    ui->Map_file_name_line_edit->setPlaceholderText(filename);
}


void MainWindow::on_export_map_clicked()
{
    Ghandler->Map->saveToCSV();
}

void MainWindow::on_clear_clicked()
{
    //Ghandler->clearScene();
}

void MainWindow::on_Update_Map_button_clicked()
{
    Ghandler->resize_map(ui->Gsize_line_edit->text().toInt());
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
    Ghandler->Map->change_type(1);
    for(int i=0; i < Ghandler->Gsize ;i++){
        for(int j=0; j < Ghandler->Gsize ;j++){
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
    Ghandler->Map->change_type(0);
    for(int i=0; i < Ghandler->Gsize ;i++){
        for(int j=0; j < Ghandler->Gsize ;j++){
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
    Ghandler->Zoom(true);
}


void MainWindow::on_Zoom_out_clicked()
{
    Ghandler->Zoom(false);
}



void MainWindow::on_actionLoad_map_triggered()
{
    MainWindow::on_load_map_clicked();
}



void MainWindow::on_actionexport_map_triggered()
{
    MainWindow::on_export_map_clicked();
}

