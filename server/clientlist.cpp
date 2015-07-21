#include "clientlist.h"
#include "ui_clientlist.h"

//Constructor
ClientList::ClientList(QWidget *parent, MyTcpServer *server, QString serverDetails) :
    QMainWindow(parent),
    ui(new Ui::ClientList)
{
    ui->setupUi(this);
    model = new QStandardItemModel;
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Client IP and Port")));    //Setting header
    ui->clientList->setModel(model);
    ui->clientList->resizeColumnsToContents();
    ui->clientList->adjustSize();
    this->server = server;

    //Whenever client is connected or disconnected update the list
    connect(server, SIGNAL(clientChanged()), this, SLOT(updateList()));

    //Show server details
    ui->serverLable->setText(serverDetails);
}

ClientList::~ClientList()
{
    delete ui;
}

//SLOT update list. Adds the details of the client
void ClientList::updateList() {
    int i;
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Client IP and Port")));
    for (i = 0; i < this->server->clientList.size(); i++) {
        model->appendRow(new QStandardItem(this->server->clientList.at(i)->peerAddress().toString() + " " + QString::number(this->server->clientList.at(i)->peerPort())));
    }
    ui->clientList->setModel(model);
    ui->clientList->resizeColumnsToContents();
    ui->clientList->adjustSize();
}
