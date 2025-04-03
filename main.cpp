#include "mainwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/AV_icon.ico"));

    QDir dir(QCoreApplication::applicationDirPath());
    if (!dir.exists("algorithms")) {
        dir.mkdir("algorithms");
    }
    if (!dir.exists("maps")) {
        dir.mkdir("maps");
    }

    MainWindow w;
    w.show();
    return a.exec();
}
