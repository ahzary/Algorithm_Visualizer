#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QIcon>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    // Enable antialiasing for smoother graphics
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->stepDelay_double_spinbox->setSuffix(" sec");
    //setup led
    algLed = ui->algorithm_ready_led;
    algLed->setFixedSize(16, 16);
    algLed->setStyleSheet(
            "QLabel { background-color: red; border-radius: 8px; border: 1px solid black; }");
    algLed->setText("");
    //create the map and give it to scene handler class
    Map = std::make_shared<GMap>(30,20);
    stopwatch_ = std::make_unique<Stopwatch>();
    Ghandler = std::make_shared<GraphicsSceneHandler>(scene
                                ,ui->graphicsView->maximumViewportSize().width()
                                ,ui->graphicsView->maximumViewportSize().height()
                                ,Map
                                ,this);
    execution_time = ui->execution_time_lineedit;
    intialize_line_edits();
    updateTimer = new QTimer(this);
    updateTimer->setInterval(100);
    scriptLoader_ = std::make_shared<scriptLoader>(Map);
    // connect Signals
    connect(Map.get(), &GMap::changeStartSquareText, this, &MainWindow::handleStartSquareText);
    connect(Map.get(), &GMap::changeEndSquareText, this, &MainWindow::handleEndSquareText);
    connect(scriptLoader_.get(), &scriptLoader::stepsTaken, this, &MainWindow::handleStepsTaken);
    connect(scriptLoader_.get(), &scriptLoader::nodesVisited, this, &MainWindow::handleNodesVisited);
    connect(scriptLoader_.get(), &scriptLoader::pathDistanceSignal, this, &MainWindow::handlePathDistance);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateStopwatch);
    connect(scriptLoader_.get(), &scriptLoader::endReached, this, &MainWindow::stopStopwatch);

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
    execution_time->setPlaceholderText("00:00.00");

}

void MainWindow::on_load_map_clicked()
{

    QString filename = QFileDialog::getOpenFileName(nullptr,
                                                    "Open CSV File",
                                                    "maps/",
                                                    "CSV Files (*.csv);;All Files (*)");
    if (filename.isEmpty()) return; // If the user cancels, do nothing

    Map->loadFromCSV(filename);
    Ghandler->draw_map();

    // get the filename from the file path and display it on the line text edit
    ui->Map_file_name_line_edit->setPlaceholderText(filename.mid(filename.lastIndexOf('/') + 1));
    ui->Gsize_line_edit->setText(QString::number(Map->getGridSize()));
    ui->Ssize_line_edit->setText(QString::number(Map->getSquareSize()));
    handleStartSquareText(0,0);
    handleEndSquareText(0,0);

}


void MainWindow::on_export_map_clicked()
{
    Map->saveToCSV();
}



void MainWindow::on_Update_Map_button_clicked()
{
    int Gsize,Ssize;

    // if there are grid size or square size changes then we resize;
    if (!ui->Ssize_line_edit->text().isEmpty()){
        Ssize =  ui->Ssize_line_edit->text().toInt();
    } else{
        Ssize = Map->getSquareSize();
    }
    if (!ui->Gsize_line_edit->text().isEmpty()){
        Gsize = ui->Gsize_line_edit->text().toInt();
    } else {
        Gsize = Map->getGridSize();
    }

    Map->resize(Gsize , Ssize);
    Ghandler->draw_map();
    //modify text boxes
    ui->Gsize_line_edit->setText(QString::number(Map->getGridSize()));
    ui->Ssize_line_edit->setText(QString::number(Map->getSquareSize()));
}

void MainWindow::on_update_square_size_button_clicked()
{
    static int Ssize=15;
    // if there are square size changes then we resize;
    if (!ui->Ssize_line_edit->text().isEmpty()){
        Ssize =  ui->Ssize_line_edit->text().toInt();
    } else{
        Ssize = Map->getSquareSize();
    }
    Map->setSquareSize(Ssize);

}


void MainWindow::on_actionexit_triggered()
{
    QApplication::quit();
}



// Map control buttons
void MainWindow::on_set_start_square_clicked()
{
    Map->change_type(5);

}
void MainWindow::on_set_end_square_clicked()
{
    Map->change_type(6);
}
void MainWindow::on_set_obstacle_square_clicked()
{
    Map->change_type(4);
}
void MainWindow::on_set_empty_sqaure_clicked()
{
    Map->change_type(1);
}

void MainWindow::on_empty_all_squares_clicked()
{
    int Gsize = Map->getGridSize();
    Map->change_type(1);
    for(int i=0; i < Gsize ;i++){
        for(int j=0; j < Gsize ;j++){
            if (Map->Grid[i][j] != nullptr) {
                Map->Grid[i][j]->type=Global_button_square_type;
                Map->Grid[i][j]->update_color();
                Map->Grid[i][j]->setPen(QPen(Qt::black));
                Map->type_grid[i][j] = Global_button_square_type;
            }
        }
    }
}


void MainWindow::on_gray_all_squares_clicked()
{
    int Gsize = Map->getGridSize();
    Map->change_type(0);
    for(int i=0; i < Gsize ;i++){
        for(int j=0; j < Gsize ;j++){
            if (Map->Grid[i][j] != nullptr) {
                Map->Grid[i][j]->type=Global_button_square_type;
                Map->Grid[i][j]->update_color();
                Map->Grid[i][j]->setPen(QPen(Qt::black));
                Map->type_grid[i][j] = Global_button_square_type;
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
        ui->steps_taken_lineedit->setPlaceholderText("0");
        ui->nodes_visited_lineedit->setPlaceholderText("0");
        ui->execution_time_lineedit->setPlaceholderText("00:00.00");
        ui->path_distance_lineedit->setPlaceholderText("0");
        ledtoggle(algLed,true);
        ui->start_pause_button->setEnabled(true);
        ui->clear->setEnabled(true);
        ui->step_once->setEnabled(true);
        ui->Algorithm_Load->setText("load");
    }

}


void MainWindow::on_start_pause_button_clicked()
{

    if(!scriptLoader_->isRunning()){

        Map->backup_map();
        scriptLoader_->startAlgorithm();
        stopwatch_->start();
        updateTimer->start();

    }else if(!scriptLoader_->isPaused()){
        scriptLoader_->pauseAlgorithm();
        stopwatch_->stop();
        updateTimer->stop();
    }else {
        scriptLoader_->resumeAlgorithm();
        stopwatch_->start();
        updateTimer->start();
    }
}

void MainWindow::on_clear_clicked()
{
    stopwatch_->reset();
    scriptLoader_->stopAlgorithm();
    ledtoggle(algLed,false);
    ui->start_pause_button->setEnabled(false);
    ui->clear->setEnabled(true);
    ui->step_once->setEnabled(false);
    Map->reset_map();
    scene->update();
}
void MainWindow::handleStartSquareText(int x, int y){
    ui->start_square_line_edit->setText(Map->get_start_square_txt());
    if (scriptLoader_->isLoaded() ){
        ui->Algorithm_Load->setText("Update");
        ui->start_pause_button->setEnabled(false);
        ui->clear->setEnabled(true);
        ui->step_once->setEnabled(false);
    }
    Ghandler->update();

}
void MainWindow::handleEndSquareText(int x, int y){
    ui->end_square_line_edit->setText(Map->get_end_square_txt());
    if (scriptLoader_->isLoaded() ){
        ui->Algorithm_Load->setText("Update");
        ui->start_pause_button->setEnabled(false);
        ui->clear->setEnabled(true);
        ui->step_once->setEnabled(false);
    }
    Ghandler->update();
}
void MainWindow::handleStepsTaken(int n){
    ui->steps_taken_lineedit->setText(QString::number(n));
    Ghandler->update();

}
void MainWindow::handleNodesVisited(int n){
    ui->nodes_visited_lineedit->setText(QString::number(n));
}
void MainWindow::on_stepDelay_double_spinbox_valueChanged(double arg1)
{
    if(arg1 !=0){
        scriptLoader_->setStepDelay(arg1);
    }

}

void MainWindow::on_step_once_clicked()
{
    scriptLoader_->pauseAlgorithm();
    scriptLoader_->Step();
}

void MainWindow::ledtoggle(QLabel* led, bool isOn) {
    led->setStyleSheet(QString("QLabel {"
                               "background-color: %1;"
                               "border-radius: %2px;"
                               "border: 1px solid black;"
                               "}")
                           .arg(isOn ? "green" : "red")
                           .arg(led->width() / 2));
}
void MainWindow::handlePathDistance(int n){
    ui->path_distance_lineedit->setText(QString::number(n));
}
void MainWindow::updateStopwatch(){
    double seconds = stopwatch_->elapsedSeconds();

    int minutes = static_cast<int>(seconds / 60);
    int sec = static_cast<int>(seconds) % 60;
    int msec = static_cast<int>((seconds - static_cast<int>(seconds)) * 1000);

    QString timeStr = QString("%1:%2.%3")
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(sec, 2, 10, QChar('0'))
                          .arg(msec, 3, 10, QChar('0'));

    execution_time->setText(timeStr);
}
void MainWindow::stopStopwatch(){
    stopwatch_->stop();

    double seconds = stopwatch_->elapsedSeconds();

    int minutes = static_cast<int>(seconds / 60);
    int sec = static_cast<int>(seconds) % 60;
    int msec = static_cast<int>((seconds - static_cast<int>(seconds)) * 1000);

    QString timeStr = QString("%1:%2.%3")
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(sec, 2, 10, QChar('0'))
                          .arg(msec, 3, 10, QChar('0'));

    execution_time->setText(timeStr);
}

void MainWindow::on_actionopen_Help_triggered()
{
    if (helpWindow && helpWindow->isVisible()) {
        helpWindow->raise();
        helpWindow->activateWindow();
        return;
    }

    QFile file("README.md");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open README.md");
        return;
    }

    QString markdownContent = file.readAll();
    file.close();

    helpWindow = new QDialog(this);
    helpWindow->setAttribute(Qt::WA_DeleteOnClose);
    helpWindow->setWindowTitle("Help");

    QTextBrowser *viewer = new QTextBrowser(helpWindow);
    viewer->setOpenExternalLinks(true);
    viewer->setOpenLinks(true);
    viewer->setMarkdown(markdownContent);
    QVBoxLayout *layout = new QVBoxLayout(helpWindow);
    layout->addWidget(viewer);
    helpWindow->setLayout(layout);
    helpWindow->resize(600, 400);
    helpWindow->show();

    // Reset pointer when closed
    connect(helpWindow, &QDialog::destroyed, this, [this]() {
        helpWindow = nullptr;
    });
}
