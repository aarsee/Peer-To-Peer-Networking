#include "downloadwindow.h"
#include "ui_downloadwindow.h"

#include <QFile>
#include <QFileDialog>
#include <qdebug.h>
#include <miniserver.h>

//Construoctor
DownloadWindow::DownloadWindow(client *client, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DownloadWindow)
{
    ui->setupUi(this);
    this->clnt = client;                        //Client who created this window

    //Setting upn download bar
    ui->downloadBar->setVisible(false);
    ui->downloadBar->setRange(0, 100);
    ui->downloadBar->setValue(0);
}

//Destructor
DownloadWindow::~DownloadWindow()
{
    delete ui;
}

//Called on "browse" button click event
void DownloadWindow::on_browse_clicked()
{
    //Select a propoer torrent file to download its source file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a torrent file"),
                                                    "./torrent/",
                                                    tr("Torrents (*.torrent);; All files (*.*)"));

    if (fileName.isEmpty())
        return;
    this->filename = fileName;
    ui->lineEdit->setText(this->filename);
}

//Called on "start downlaod" button click event
void DownloadWindow::on_start_downloading_clicked()
{
    if (!ui->lineEdit->text().isEmpty()) {
        ui->downloadBar->setVisible(true);

        //Creating miniserver to download a file
        MiniServer *server = new MiniServer(this->clnt->editName(ui->lineEdit->text()), this);
        QString res = server->initialize();
        connect(server, SIGNAL(dataGot()), this, SLOT(valueChanged()));                         //As soon as data got, uptade progress bar
        QString data = " C " + res + " ";
        this->clnt->serverList.append(server);

        //Send central server a request to dowanload file with miniserver's ip and port and filename
        this->clnt->socket->write(data.toUtf8());
        connect(server, SIGNAL(fileTransferDone()), this, SLOT(fileTransferDone()));            //Connect fileTransferDone signal
    }
}

//Called as soon as some data is written to the file
void DownloadWindow::valueChanged() {
    //qDebug() << ((MiniServer *)sender())->numberOfBytesWritten << " " << ((MiniServer *)sender())->size;
    ui->downloadBar->setValue((int)(((float)((MiniServer *)sender())->numberOfBytesWritten/((MiniServer *)sender())->size)*100));
}

//Called as soon as miniserver notifies file transfer is done
void DownloadWindow::fileTransferDone() {
    MiniServer *server = (MiniServer *)sender();
    QString finish = " F " + server->serverDetails + " ";   //Tell central server that file transfer is done
    this->clnt->socket->write(finish.toUtf8());
    this->clnt->socket->waitForBytesWritten();
    int i = 0;
    for (; i < this->clnt->serverList.size(); i++) {        //Remove miniserver from list of miniservers
        if (this->clnt->serverList.at(i) == server) {
            this->clnt->serverList.removeAt(i);
            break;
        }
    }
}
