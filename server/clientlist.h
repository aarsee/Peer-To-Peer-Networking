#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "mytcpserver.h"

namespace Ui {
class ClientList;
}

class ClientList : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientList(QWidget *parent = 0, MyTcpServer *server = 0, QString serverDetails = "");
    ~ClientList();

public slots:
    void updateList();

private:
    Ui::ClientList *ui;
    QStandardItemModel *model;
    MyTcpServer *server;
};

#endif // CLIENTLIST_H
