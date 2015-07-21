#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;               //Create a window and show it
    w.show();
    return a.exec();            //Starts main thread
}
