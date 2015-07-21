#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;                   //Mainwindow is created and shown
    w.show();

    return a.exec();                //starts the main thread
}
