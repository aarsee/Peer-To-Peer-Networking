#include "client.h"

// Structure to maintain data of all sockets which uploads a file
struct socketData {
    qint64 socketDesciptor;
    QString filename;
};

client::client(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket;
    commandReading = false;
    startReading = false;
    endReading = false;
    ipReading = false;
    portReading = false;
    filenameReading = false;
}

//Connection to central server
void client::connectHost(QString IP, qint64 port) {
    socket->connectToHost(IP, port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));             //Connecting readyRead signal
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));       //Connecting disconnect signal
}

//Whenever the socket is disconnected, remove it from list and it'll be deleted after its all processes are done
void client::disconnected() {
    QTcpSocket *socket = (QTcpSocket *)sender();
    int i;
    for (i = 0; i < socDataList.size(); i++) {
        if (socDataList.at(i)->socketDesciptor == socket->socketDescriptor()) {
            socDataList.removeAt(i);
            break;
        }
    }
    socket->deleteLater();
}

//SLOT for readyRead signal. Called whenever data arrives on the socket
void client::readyRead() {
    QTcpSocket *socket = (QTcpSocket *)sender();
    QByteArray Data = socket->readAll();
    qDebug() << QString(Data);
    int i = 0;

    //Data is coming serially so we're using protocols to read command(which tells what to do) and do things accordignly
    for (i = 0; i < Data.size(); i++) {
        if (!commandReading && !startReading && !endReading && !ipReading && !portReading && !filenameReading) {
            commandReading = true;
            command.clear();
            start.clear();              //Start byte buffer
            end.clear();                //End byte buffer
            startReading = false;       //Start byte reading or not
            endReading = false;         //End byte reading or not
            ip.clear();                 //Ip address buffer for other peer's miniserver
            port.clear();               //Port address buffer for other peer's miniserver
            filename.clear();           //file buffer for other peer's download request
            ipReading = false;          //Ip reading or not
            portReading = false;        //Port reading or not
            filenameReading = false;    //Filename reading or not
            continue;
        }

        if (commandReading) {
            if (char(Data[i]) != ' ') {                         //Reading command till ' ' comes because information is seperated by space
                command.append(Data[i]);
                continue;
            } else {
                commandReading = false;                         //Command reading is over
                if (!QString(command).compare("R")) {           //If command is 'R' then someone is requesting data so next information is start byte
                    startReading = true;
                    start.clear();
                } else if (!QString(command).compare("C")) {    //If command is 'C' then central server is asking to connect to a peer's miniserver so next is IP
                    ipReading = true;
                    ip.clear();
                } else if (!QString(command).compare("S")) {    //If command id 'S' then central server has confirmed the connection
                    emit clientConnected();                     //Emits clientConnected signal
                }
                continue;
            }
        }

        if (!QString(command).compare("R")) {
            if (startReading) {
                if (char(Data[i]) != ' ') {                     //Read start byte till ' ' comes
                    start.append(Data[i]);
                    continue;
                } else {
                    startReading = false;                       //Start byte reading is done so next is End byte
                    endReading = true;
                    end.clear();
                    continue;
                }
            }

            if (endReading) {
                if (char(Data[i]) != ' ') {                         //End byte reading till ' ' comes
                    end.append(Data[i]);
                    continue;
                } else {                                            //End reading is over so next is to send data
                    endReading = false;
                    command.clear();
                    QTcpSocket *socket = (QTcpSocket *)sender();    //Getting the socket which called this slot
                    int l = 0;
                    QString fname;
                    //Getting corresponding filename for that socket
                    for (; l < socDataList.size(); l++) {
                        if (socDataList.at(l)->socketDesciptor == socket->socketDescriptor()) {
                            fname = socDataList.at(l)->filename;
                            break;
                        }
                    }
                    //If found then call sendFile function with filename, socket, start byte and end byte
                    if (l < socDataList.size() && socDataList.size() != 0)
                        sendFile(fname, socket, (qint64)start.toLong(), (qint64)end.toLong());
                    continue;
                }
            }
        } else if (!QString(command).compare("C")) {
            if (ipReading) {                                            //Command is 'C' so start reading IP till ' ' comes
                if (char(Data[i]) != ' ') {
                    ip.append(Data[i]);
                    continue;
                } else {
                    ipReading = false;                                  //IP reading is done so next is port
                    portReading = true;
                    port.clear();
                    continue;
                }
            }

            if (portReading) {
                if (char(Data[i]) != ' ') {                             //Read port till ' ' comes
                    port.append(Data[i]);
                    continue;
                } else {
                    portReading = false;                                //Port reading is done so next is filename
                    filenameReading = true;
                    filename.clear();
                    continue;
                }
            }

            if (filenameReading) {
                //filename reading till ' ' comes. Torrent filename is hex hash of source file so no ' ' character in filename
                if (char(Data[i]) != ' ') {
                    filename.append(Data[i]);
                    continue;
                } else {
                    filenameReading = false;
                    QTcpSocket *socket = new QTcpSocket();
                    socket->connectToHost(QString(ip), port.toInt());               //Connecting to peer's miniserver

                    //Connecting signals
                    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
                    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

                    socketData *socData = new socketData;
                    socData->socketDesciptor = socket->socketDescriptor();
                    socData->filename = getNameFromTorrent(QString(filename));      //get source file name using getNameFromTorrent function
                    socDataList.append(socData);                                    //Added socket data to the list which helds filename info for that socket
                    continue;
                }
            }

        }
    }
}

//Open file with filename @filename and read bytes from @start to @end and write those bytes to socket which will send data
void client::sendFile(QString filename, QTcpSocket *socket, qint64 start, qint64 end) {
    QFile inputFile(filename);
    QByteArray read;
    if (inputFile.exists()) {                               //If file exists then open it
        inputFile.open(QIODevice::ReadOnly);
    } else {
        return;
    }

    inputFile.seek(start);                                  //Go to start byte
    read = inputFile.read(end - start + 1);
    QString data = " " + QString::number(start) + " ";      //Creating message with start byte to send
    socket->write(data.toUtf8());
    socket->waitForBytesWritten();

    //If the data read is less than required data then sending garbage data so that protocol doesn't get violated
    if (read.size() != (end-start+1)) {
        read.clear();
        while (read.size() <end-start+1) {
            read.append('a');
        }
        qDebug() << end-start+1 << " " << read.size();
    }
    //Write data
    socket->write(read);
    socket->waitForBytesWritten();
    socket->write(QString(" ").toUtf8());
    socket->waitForBytesWritten();
    inputFile.close();
}

//Opens file with @filename and read valeu of 'name' keyword which is source file's name
QString client::getNameFromTorrent(QString filename) {
    QFile ifile(filename);
    QByteArray read1;
    ifile.open(QIODevice::ReadOnly);

    QString fname;
    read1 = ifile.readLine();
    while (read1.size() != 0) {                 //Reading line by line
        QString command;
        int i = 0;
        QString read(read1);
        read.truncate(read.size()-1);
        while (read.at(i) != ' ') {
            command.append(read.at(i));
            i++;
        }
        i++;
        if (!command.compare("name")) {         //Found name
            while (i < read.size()) {           //Read filename
                fname.append(read.at(i));
                i++;
            }
            ifile.close();
            return fname;                       //send filename
        }
        read1 = ifile.readLine();
    }

    ifile.close();
    return QString("");
}

//Extracts source file name from its full path
QString client::editName(QString filename) {
    QString fname;
    int len = filename.size();
    int i = len - 1;
    while (filename.at(i) != '/' && i >= 0) {       //Start reading from back till '/' comes
        fname.prepend(filename.at(i));
        i--;
    }
    return fname;                                   //send filename
}
