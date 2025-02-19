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



    void on_Update_Map_button_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    std::shared_ptr<GraphicsSceneHandler> Ghandler;
    QLineEdit* Gsize_line_edit ;
};
#endif // MAINWINDOW_H
