#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Called on connect button click event
void MainWindow::on_connect_clicked()
{
    client *clnt = new client;
    clnt->connectHost(ui->ipAddr->text(), ui->portNo->text().toInt());       //Connect to host
    this->clnt = clnt;
    connect(clnt, SIGNAL(clientConnected()), this, SLOT(clientConnected())); //Signal to wait till confirmatio of client connection to server
}

//SLOT for clientConnected signal. Client emits it when server confirms connection
void MainWindow::clientConnected()
{
    KeyWindow *kWindow = new KeyWindow(this->clnt);                         //Initialize key-window and shows it
    kWindow->show();
    this->close();                                                          //Closes current window
}
