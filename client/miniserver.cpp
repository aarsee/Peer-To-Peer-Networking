#include "miniserver.h"
#include <fstream>

using namespace std;

//Structure to manage file download from other clients
struct socketData {
    qint64 socketDesciptor;
    bool readingStart;
    bool readingHash;
    bool dataRead;
    QByteArray start;
    qint64 end;
    QByteArray buffer;
    QByteArray hash;
};

//Constructor which passes torrent filename and it is parsed in this function
MiniServer::MiniServer(QString filename, QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    this->sizechk = 0;
    this->numberOfBytesWritten = 0;
    this->tfilename = filename;
    this->parseFile(filename);
    QFile file(this->filename);
    if(!(file.open(QIODevice::Append)))
    {
       qDebug("File cannot be opened.");
       exit(0);
    }
    file.close();
}

//Initializes a server and connect signals
QString MiniServer::initialize() {
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));      //called when New connection comes
    connect(server, SIGNAL(destroyed()), this, SLOT(deleteLater()));            //called when server closes down

    if (!server->listen()) {                                                    //Start listening
        QString error = "Server can not be started: ";
        error += server->errorString();
        return error;
    } else {

        //Get IPv4 Address and returns server details
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
        success += QString::number(server->serverPort());
        success += tr(" ");
        success += this->tfilename;
        this->serverDetails = success;
        return success;
    }
}

//SLOT for newConnection. Called on new connection
void MiniServer::newConnection() {
    QTcpSocket *socket = server->nextPendingConnection();       //Returns the socket of the client that attempted to connect
    socketData *data = new socketData;                          //Create socket data to maintain data regarding that socket
    data->socketDesciptor = socket->socketDescriptor();
    data->dataRead = false;
    data->end = 0;
    data->readingStart = false;
    data->start = 0;
    data->readingHash = false;

    socDataList.append(data);                                   //Add data to socDataList
    clientList.append(socket);                                  //Store all the sockets to clientList

    //Connection of signals
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    //qDebug() << socket->peerAddress().toString() << " " << socket->peerPort();

    //Issue first data request to the client according to "this->sizechk". Next request will be issued upon data arrival from that socket
    if(this->sizechk < this->size) {
        qint64 end = this->size < (this->sizechk + this->pieceSize - 1) ? this->size : (this->sizechk + this->pieceSize - 1);
        QString request = " R " + QString::number(this->sizechk) + " " + QString::number(end) + " ";
        this->sizechk = end + 1;
        socket->write(request.toUtf8());
        socket->waitForBytesWritten();
        //qDebug() << request;
    }
}

//SLOT to readyRead. Called upon data arrival
void MiniServer::readyRead() {
    QTcpSocket *socket = (QTcpSocket *)sender();                                //Get the socket that sent data
    int p = 0, len = socDataList.size();
    socketData *socDes = NULL;
    for ( ; p < len; p++) {                                                     //Get socDes according to that socket
        if (socDataList.at(p)->socketDesciptor == socket->socketDescriptor()) {
            socDes = socDataList.at(p);
            break;
        }
    }
    QByteArray Data = socket->readAll();                //Read data from the buffer that comes on the socket
    int i = 0;

    //Process the message which will start with start byte and then data
    for (i = 0; i < Data.size(); i++) {
        //If dataRead is on then read data into buffer till buffer's size is matched to expected size
        if (!socDes->readingStart && socDes->dataRead && socDes->buffer.size() < (socDes->end-(qint64)(socDes->start.toLong())+1)) {
            socDes->buffer.append(Data[i]);
            continue;
        }

        //If expected data is in buffer then ....
        if ((socDes->buffer.size() == (socDes->end - (qint64)(socDes->start.toLong()) + 1)) && socDes->dataRead) {
            //qDebug() << "got";
            fstream file;
            file.open(this->filename.toUtf8().constData(), ios::in | ios::out | ios::ate);
            if (!map[(qint64)(socDes->start.toLong())]) {           //If data is already not written

                QCryptographicHash *ha = new QCryptographicHash(QCryptographicHash::Md5);
                ha->addData(socDes->buffer.constData());
                QString hash = QString(ha->result().toHex());

                if (!hash.compare(this->pieceHashes.at(((qint64)socDes->start.toLong())/this->pieceSize))) {        //Check hash of the piece
                        //Mark it as data written and write data and emit signal that data is written
                        map[(qint64)(socDes->start.toLong())] = true;
                        file.seekp(socDes->start.toLong(), ios::beg);
                        file.write(socDes->buffer.constData(), socDes->buffer.size());
                        this->numberOfBytesWritten += socDes->buffer.size();    //Update number of bytes written
                        emit dataGot();
                        if (this->numberOfBytesWritten == this->size) {
                            file.close();                                       //Check if file transfer is done
                            emit fileTransferDone();
                        }
                } else {

                    //If piece is dirty then ask the request the next client to send data.
                    qint64 end = this->size-1 < ((qint64)socDes->start.toLong() + this->pieceSize - 1) ? this->size-1 : ((qint64)socDes->start.toLong() + this->pieceSize - 1);
                    QString request = " R " + QString::number((qint64)socDes->start.toLong()) + " " + QString::number(end) + " ";

                    int i = 0;
                    for ( ; i < clientList.size(); i++) {
                        if (clientList.at(i)->socketDescriptor() == socket->socketDescriptor())
                            break;
                    }
                    i = (i + 1) % clientList.size();
                    clientList.at(i)->write(request.toUtf8());
                    clientList.at(i)->waitForBytesWritten();
                    file.close();
                    continue;
                }
            }

            //Issue next request till sizechk is less than size
            if(this->sizechk < this->size) {
                qint64 end = this->size-1 < (this->sizechk + this->pieceSize - 1) ? this->size-1 : (this->sizechk + this->pieceSize - 1);
                QString request = " R " + QString::number(this->sizechk) + " " + QString::number(end) + " ";
                this->sizechk = end + 1;
                socket->write(request.toUtf8());
                socket->waitForBytesWritten();
            } else {

                //Else check if there is some data missing and request that data
                qint64 start = 0;
                while (start < this->size) {
                    if (!map[start]) {
                        qint64 end = this->size-1 < (start + this->pieceSize - 1) ? (this->size - 1) : (start + this->pieceSize - 1);
                        QString request = " R " + QString::number(start) + " " + QString::number(end) + " ";
                        socket->write(request.toUtf8());
                        socket->waitForBytesWritten();
                        break;
                    }
                    start += this->pieceSize;
                }
            }

            file.close();
            socDes->buffer.clear();
            socDes->dataRead = false;
            continue;
        }
        if (!socDes->readingStart) {                    //Start reading start byte
            socDes->start.clear();
            socDes->readingStart = true;
            socDes->dataRead = false;
            continue;
        }
        if (socDes->readingStart) {                     //start reading start till ' ' comes
            if (char(Data[i]) != ' ') {
                socDes->start.append(Data[i]);
                continue;
            }
            else {
                socDes->readingStart = false;           //Decide end byte and make dataRead true
                socDes->end = (this->size - 1) < ((qint64)(socDes->start.toLong()) + this->pieceSize - 1) ? (this->size - 1) : (qint64)(socDes->start.toLong()) + this->pieceSize - 1;
                socDes->dataRead = true;
                continue;
            }
        }
    }
}

//SLOT for disconnect signal
void MiniServer::disconnected() {
    int i;
    qint64 socketDescriptor = ((QTcpSocket *)sender())->socketDescriptor();
    for (i = 0; i < socDataList.size(); i++) {                                  //Remove from socDataList and clientList
        if (socDataList.at(i)->socketDesciptor == socketDescriptor) {
            socDataList.removeAt(i);
            clientList.removeAt(i);
            break;
        }
    }
    ((QTcpSocket *)sender())->deleteLater();
}

//Parsing torrent file
void MiniServer::parseFile(QString filename) {
    QFile ifile(filename);
    QByteArray read1;
    ifile.open(QIODevice::ReadOnly);

    read1 = ifile.readLine();
    while (read1.size() != 0) {
        QString command;
        int i = 0;
        QString read(read1);
        read.truncate(read.size()-1);
        while (read.at(i) != ' ') {
            command.append(read.at(i));
            i++;
        }
        i++;
        if (!command.compare("name")) {                 //Reading name
            while (i < read.size()) {
                this->filename.append(read.at(i));
                i++;
            }
        } else if (!command.compare("piece-length")) {  //Reading piece length
            QString num;
            while (i < read.size()) {
                num.append(read.at(i));
                i++;
            }
            this->pieceSize = (qint64)num.toLong();
        } else if (!command.compare("size")) {          //Reading piece size
            QString num;
            while (i < read.size()) {
                num.append(read.at(i));
                i++;
            }
            this->size = (qint64)num.toLong();
        } else if (!command.compare("piece-hash")) {    //Reading number of piece hashes
            QString num;
            while (i < read.size()) {
                num.append(read.at(i));
                i++;
            }
            qint64 pieces = (qint64)num.toLong();
            int j = 0;
            for ( ; j < pieces; j++) {                  //Reading piece hashes
                read1 = ifile.readLine();
                QString line(read1);
                line.truncate(line.size() - 1);
                this->pieceHashes.append(line);
            }
        }
        read1 = ifile.readLine();
    }

    ifile.close();
}
