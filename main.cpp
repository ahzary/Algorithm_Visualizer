#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //
    //std::shared_ptr<GraphicsAreaHandler> s = std::make_shared<GraphicsAreaHandler>();
    MainWindow w;
    w.show();
    return a.exec();
}
