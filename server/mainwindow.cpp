#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mytcpserver.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusLabel->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Called on "Start Server" button click event
void MainWindow::on_startServerButton_clicked()
{
    MyTcpServer *tcpServer = new MyTcpServer;
    QString response = tcpServer->initialize();                 //Initialize tcpserver
    ui->statusLabel->setText(response);                         //Show server details
    ui->statusLabel->adjustSize();
    ClientList *list = new ClientList(0, tcpServer, response);  //Create a client-list for the server
    this->close();
    list->show();                                               //Show clientlist window which shows list of clients
}
