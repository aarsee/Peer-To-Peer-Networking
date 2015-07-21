//Main window which has download and create torrent buttons

#include "keywindow.h"
#include "ui_keywindow.h"
#include "downloadwindow.h"
#include "createtorrentwindow.h"

//Constructor
KeyWindow::KeyWindow(client *client, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KeyWindow)
{
    ui->setupUi(this);
    this->clnt = client;
}

KeyWindow::~KeyWindow()
{
    delete ui;
}

//Called on "Download" button click event
void KeyWindow::on_download_clicked()
{
    DownloadWindow *dWindow = new DownloadWindow(this->clnt);
    dWindow->show();
}

//Called on "Create Torrent" button click event
void KeyWindow::on_create_torrent_clicked()
{
    CreateTorrentWindow *tWindow = new CreateTorrentWindow(this->clnt);
    tWindow->show();
}
