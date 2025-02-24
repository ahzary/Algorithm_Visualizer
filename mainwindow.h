#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "graphicsscenehandler.h"
#include <QIntValidator>
#include <QLineEdit>


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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    std::shared_ptr<GraphicsSceneHandler> Ghandler;
    QLineEdit* Gsize_line_edit ;
};
#endif // MAINWINDOW_H
