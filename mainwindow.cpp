#include "mainwindow.h"
#include "./ui_mainwindow.h"


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

}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_load_map_clicked()
{
        Ghandler->addrect();
}


void MainWindow::on_clear_clicked()
{
     Ghandler->clearScene();
}

void MainWindow::on_Update_Map_button_clicked()
{
    Ghandler->resize_map(ui->Gsize_line_edit->text().toInt());
}

