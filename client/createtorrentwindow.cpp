#include "createtorrentwindow.h"
#include "ui_createtorrentwindow.h"

#include <QFile>
#include <QCryptographicHash>
#include <QFileDialog>

CreateTorrentWindow::CreateTorrentWindow(client *clnt, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateTorrentWindow)
{
    ui->setupUi(this);
    this->clnt = clnt;                      //Client which created this window
    ui->progressBar->setVisible(false);     //Progress bar is not visible in the starting
}

CreateTorrentWindow::~CreateTorrentWindow()
{
    delete ui;
}

void CreateTorrentWindow::on_browse_clicked()
{
    QFileDialog *fdg = new QFileDialog;

    //Choose a file to create a torrent file of that file
    QString fileName = fdg->getOpenFileName(this, tr("Choose a file to create torrent file"),
                                           ".",
                                           tr("All files (*.*)"));

    if (fileName.isEmpty())
        return;
    this->filename = fileName;
    ui->lineEdit->setText(this->filename);      //Set that filename in textEdit
}

//Called on "Create" button clicked event
void CreateTorrentWindow::on_create_clicked()
{
    if (!ui->lineEdit->text().isEmpty()) {
        createFile(ui->lineEdit->text());       //Call createFile method with filename of the source file
    }
}

void CreateTorrentWindow::createFile(QString filename) {
    //Setup progress bar
    ui->progressBar->setVisible(true);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    QFile ifile(filename);
    QByteArray read;
    ifile.open(QIODevice::ReadOnly);

    QCryptographicHash *h = new QCryptographicHash(QCryptographicHash::Md5);
    h->addData(ifile.readAll().constData());

    //Setting filename as hash of the file
    QString fname1 = QString(h->result().toHex()) + ".torrent";
    QFile tfile(fname1.toUtf8().constData());
    ifile.seek(0);
    if(!(tfile.open(QIODevice::Append)))
    {
       qDebug("File cannot be opened.");
       exit(0);
    }

    //Writing "name " followed by the source filename
    tfile.write("name ");
    QString name = this->clnt->editName(ifile.fileName());

    tfile.write(name.toUtf8().constData());
    tfile.write(QString(tr("\n")).toUtf8().constData());

    //Writing "piece-length " followed by length og the piece which is 256000
    tfile.write("piece-length ");
    tfile.write("256000");
    tfile.write(QString(tr("\n")).toUtf8().constData());

    //Writing "size " followed by size of file
    tfile.write("size ");
    tfile.write(QString::number(ifile.size()).toUtf8().constData());
    tfile.write(QString(tr("\n")).toUtf8().constData());

    //Writing number of pieces and piece hashes of all the pieces
    tfile.write("piece-hash ");
    qint64 pieces = ifile.size()/256000;
    if (pieces % 256000 != 0 || pieces == 0)
        pieces++;
    tfile.write(QString::number(pieces).toUtf8().constData());
    tfile.write(QString(tr("\n")).toUtf8().constData());

    int i;
    for (i = 0; i < pieces; i++) {
        ui->progressBar->setValue((i+1/pieces)*100);                                    //Updating progress bar
        read = ifile.read(256000);
        QCryptographicHash *ha = new QCryptographicHash(QCryptographicHash::Md5);
        ha->addData(read.constData());
        //ha->reset();
        QString hash = QString(ha->result().toHex());
        tfile.write(hash.toUtf8().constData());
        tfile.write(QString(tr("\n")).toUtf8().constData());
    }
    ui->progressBar->setValue((i/pieces)*100);
    tfile.close();
    QString finish = " F " + getAddress() + " " + fname1 + " ";                         //Notifying central server about creation
    this->clnt->socket->write(finish.toUtf8());
    this->clnt->socket->waitForBytesWritten();
    //qDebug() << finish ;
}

//Returns IPv4Address and if it is not available then localhost and appending port as 0
QString CreateTorrentWindow::getAddress(){
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    QString success = ipAddress;
    success += tr(" ");
    success += QString::number(00000);
    return success;
}
