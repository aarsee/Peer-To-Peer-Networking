#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <keywindow.h>
#include <client.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_connect_clicked();
    void clientConnected();

private:
    Ui::MainWindow *ui;
    client *clnt;
};

#endif // MAINWINDOW_H
