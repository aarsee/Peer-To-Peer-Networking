#include "mytcpserver.h"
#include <fstream>

//Peers list which shows which peers have particular torrent file
struct PeerList{
    QString filename;
    QList<QString> clients;
};

//Downloaders list which shows who downloads a torrent file and contains mini serverDetails too
struct DownloaderList{
    QString filename;
    QList<QTcpSocket *> downloaders;
    QList<QString> serversDetails;
};

MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    commandReading = false;
    ipReading = false;
    portReading = false;
    filenameReading = false;
}

QString MyTcpServer::initialize() {
    //Connect signals
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(server, SIGNAL(destroyed()), this, SLOT(deleteLater()));

    if (!server->listen()) {
        QString error = "Server can not be started: ";
        error += server->errorString();
        return error;
    } else {
        QString success = tr("Server is started:\n");
        success += tr("IP: ");
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

        success += ipAddress;
        success += tr("\n");
        success += tr("PORT: ");
        success += QString::number(server->serverPort());
        success += tr("\n");
        return success;                                                     //Return result of starting server
    }
}

//SLOT for new connection
void MyTcpServer::newConnection() {
    QTcpSocket *socket = server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << socket->peerAddress().toString() << " " << socket->peerPort();

    clientList.append(socket);

    socket->write(" S ");                   //Confirm the connection
    socket->waitForBytesWritten();

    emitChangeList();                           //emit change list signal

    /*Check that if the newly connected client has any file which is currently being downloaded then
    ask the client to connect to that miniserver */
    int size = databaseDownloaders.size();
    int q = 0;
    for(q = 0; q < size; q++){
        QString filey = databaseDownloaders.at(q)->filename;
        int r;
        for(r = 0; r<databasePeers.size(); r++){
            if(!databasePeers.at(r)->filename.compare(filey)){
               QString newIp = socket->peerAddress().toString();
               qDebug() << newIp;
               if(databasePeers.at(r)->clients.contains(newIp)){
                   int y;
                   for ( y = 0; y < databaseDownloaders.at(q)->downloaders.size(); y++) {
                        QString req = " C " + databaseDownloaders.at(q)->serversDetails.at(y) + " " + databaseDownloaders.at(q)->filename + " ";
                        socket->write(req.toUtf8());
                        socket->waitForBytesWritten();
                        qDebug() << req;
                   }
               }
            }
        }
    }
}

//SLOT for readyread
void MyTcpServer::readyRead() {
    QTcpSocket *socket = (QTcpSocket *)sender();
    QByteArray Data = socket->readAll();
    qDebug() << QString(Data);
    int i = 0;
    for (i = 0; i < Data.size(); i++) {
        if (!commandReading && !ipReading && !portReading && !filenameReading) {
            commandReading = true;
            command.clear();
            ip.clear();
            port.clear();
            filename.clear();
            ipReading = false;
            portReading = false;
            filenameReading = false;
            continue;
        }

        if (commandReading) {
            if (char(Data[i]) != ' ') {                     //Read command till ' ' comes
                command.append(Data[i]);
                continue;
            } else {
                commandReading = false;
                if (!QString(command).compare("C")) {           //If command is 'C' then next is ip
                    ipReading = true;
                    ip.clear();
                } else if (!QString(command).compare("F")) {    //if command is 'F' then next it ip
                    ipReading = true;
                    ip.clear();
                }
                continue;
            }
        }

        if (ipReading) {
            if (char(Data[i]) != ' ') {                 //read ip till ' ' comes
                ip.append(Data[i]);
                continue;
            } else {
                ipReading = false;                      //ip is read and next is port
                portReading = true;
                port.clear();
                continue;
            }
        }

        if (portReading) {                              //read port till ' ' comes
            if (char(Data[i]) != ' ') {
                port.append(Data[i]);
                continue;
            } else {
                portReading = false;                    //port reading is done. next is filename
                filenameReading = true;
                filename.clear();
                continue;
            }
        }

        if (filenameReading) {
            if (char(Data[i]) != ' ') {
                filename.append(Data[i]);
                continue;
            } else {
                filenameReading = false;                        //Filename reading is done
                if (!QString(command).compare("C")) {                           //If command is 'C'

                    //Go through peers database and ask peers to connect if they are online and have file
                    int p = 0, len = databasePeers.size(), len1, y;
                    for ( ; p < len; p++) {
                        if (!QString(filename).compare(databasePeers.at(p)->filename)) {
                            len1 = databasePeers.at(p)->clients.size();
                            for (y = 0; y < len1; y++) {
                                QString ipAddr = databasePeers.at(p)->clients.at(y);
                                int g = 0;
                                for (; g < clientList.size(); g++) {
                                    if (!clientList.at(g)->peerAddress().toString().compare(ipAddr)) {
                                        QString req = " C " + QString(ip) + " " + QString(port) + " " + filename + " ";
                                        clientList.at(g)->write(req.toUtf8());
                                        clientList.at(g)->waitForBytesWritten();
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    //Check the database of downloaders and ask them to connect if they have file
                    p = 0;
                    len = databaseDownloaders.size();
                    for (p = 0; p < len; p++){
                        if (!QString(filename).compare(databaseDownloaders.at(p)->filename)) {
                            QString req = " C " + QString(ip) + " " + QString(port) + " " + filename + " ";
                            int size = databaseDownloaders.at(p)->downloaders.size();
                            int q = 0;
                            for( q = 0; q < size; q++){
                                databaseDownloaders.at(p)->downloaders.at(q)->write(req.toUtf8());
                                databaseDownloaders.at(p)->downloaders.at(q)->waitForBytesWritten();
                            }
                            //Add this client to the downloaders database
                            databaseDownloaders.at(p)->downloaders.append((QTcpSocket *)sender());
                            databaseDownloaders.at(p)->serversDetails.append(QString(ip) + " " + QString(port));
                            break;
                        }
                    }

                    //if no file in downloaders database then create Downloaders list for that file and add the client
                    if (p == len) {
                        DownloaderList *d = new DownloaderList;
                        d->filename = QString(filename);
                        d->downloaders.append((QTcpSocket *)sender());
                        d->serversDetails.append(QString(ip) + " " + QString(port));
                        databaseDownloaders.append(d);
                    }
                } else if (!QString(command).compare("F")) {                //If command if 'F'
                    int z = 0, len = databaseDownloaders.size();
                    QString data = QString(ip) + " " + QString(port);

                    //Remove the client from downloaders database if it is in the database(creation if torrent file is exception)
                    for ( ; z < len; z++) {
                        if (!QString(filename).compare(databaseDownloaders.at(z)->filename) && databaseDownloaders.at(z)->serversDetails.contains(data)) {
                            int temp = 0;
                            for (; temp < databaseDownloaders.at(z)->serversDetails.size(); temp++) {
                                if (!data.compare(databaseDownloaders.at(z)->serversDetails.at(temp))) {
                                    databaseDownloaders.at(z)->serversDetails.removeAt(temp);
                                    databaseDownloaders.at(z)->downloaders.removeAt(temp);
                                    break;
                                }
                            }
                        }
                    }
                    data = QString(ip);
                    int z1 = 0, len1 = databasePeers.size();

                    //Add this client to peers database
                    for ( ; z1 < len1; z1++) {
                        if (!QString(filename).compare(databasePeers.at(z1)->filename)) {
                            databasePeers.at(z1)->clients.append(data);
                            break;
                        }
                    }

                    //If no data for this file in peers database then create a peerList for this file and add the client
                    if (z1 == len1) {
                        PeerList *d = new  PeerList;
                        d->filename = QString(filename);
                        d->clients.append(data);
                        databasePeers.append(d);
                    }
                }
                continue;
            }
        }
    }

}

//Simple function to get Ip from a string which will be of format "ip port"
QString MyTcpServer::getIp(QString data) {
    QString ip;
    int i = 0;
    while (data.at(i) != ' ') {
        ip.append(data.at(i));
        i++;
    }
    return ip;
}

//SLOT for disconnect signal
void MyTcpServer::disconnected() {
    int i;
    qint64 socketDescriptor = ((QTcpSocket *)sender())->socketDescriptor();

    //Remove from client list
    for (i = 0; i < clientList.size(); i++) {
        if (clientList.at(i)->socketDescriptor() == socketDescriptor) {
            clientList.removeAt(i);
            emitChangeList();
            break;
        }
    }

    //Remove from downloaders database
    for (i = 0; i < databaseDownloaders.size(); i++) {
        int j;
        for (j = 0; j < databaseDownloaders.at(i)->downloaders.size(); j++) {
            if (databaseDownloaders.at(i)->downloaders.at(j)->socketDescriptor() == socketDescriptor) {
                databaseDownloaders.at(i)->downloaders.removeAt(j);
                databaseDownloaders.at(i)->serversDetails.removeAt(j);
            }
        }
    }
    ((QTcpSocket *)sender())->deleteLater();
}

//Simple function to emit signal notifying list change
void MyTcpServer::emitChangeList() {
    emit clientChanged();
}
