#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QIntValidator>
#include <QLineEdit>
#include "gmap.h"
#include "graphicsscenehandler.h"
#include "scriptloader.h"
#include "stopwatch.h"
#include <QLabel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::shared_ptr<GMap> Map ;



private slots:


    void on_load_map_clicked();
    void on_clear_clicked();
    void on_actionexit_triggered();
    void on_Update_Map_button_clicked();


    // fill out all text boxes
    void intialize_line_edits();
    //Map settings buttons start __________
    void on_set_start_square_clicked();

    void on_set_end_square_clicked();

    void on_set_obstacle_square_clicked();

    void on_set_empty_sqaure_clicked();

    void on_empty_all_squares_clicked();

    void on_gray_all_squares_clicked();
    //Map settings buttons end __________

    // Zoom buttons
    void on_Zoom_in_clicked();
    void on_Zoom_out_clicked();
    void on_Zoom_Reset_clicked();

    // Map export and load
    void on_export_map_clicked();
    void on_actionLoad_map_triggered();
    void on_actionexport_map_triggered();


    // Algorithm script load
    void on_Algorithm_Load_clicked();

    void on_start_pause_button_clicked();

    void on_update_square_size_button_clicked();

    void on_stepDelay_double_spinbox_valueChanged(double arg1);

    void on_step_once_clicked();
    void ledtoggle(QLabel* led, bool isOn);

public slots:
    //Grid Map info methods
    void handleStartSquareText( int x,  int y);
    void handleEndSquareText( int x,  int y);
    void handleStepsTaken(int n);
    void handleNodesVisited(int n);
    void handlePathDistance(int n);
    void updateStopwatch();
    void stopStopwatch();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    std::shared_ptr<GraphicsSceneHandler> Ghandler;
    QLineEdit* Gsize_line_edit ;
    std::shared_ptr<scriptLoader> scriptLoader_;
    QLabel* algLed;
    std::unique_ptr<Stopwatch> stopwatch_;
    QLineEdit* execution_time;
    QTimer* updateTimer;
};
#endif // MAINWINDOW_H
