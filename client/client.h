#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QThread>
#include <QCryptographicHash>
#include <miniserver.h>

typedef struct socketData socketData;

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);
    void connectHost(QString IP, qint64 port);
    void sendFile(QString filename, QTcpSocket *socket, qint64 start, qint64 end);
    QString getNameFromTorrent (QString filename);
    QString editName(QString filename);
    QTcpSocket *socket;
    QList<socketData *> socDataList;
    QList<MiniServer *> serverList;
    bool commandReading;
    QByteArray command;
    bool startReading;
    bool endReading;
    QByteArray start;
    QByteArray end;
    bool ipReading;
    bool portReading;
    bool filenameReading;
    QByteArray ip;
    QByteArray port;
    QByteArray filename;

signals:
    void clientConnected();

public slots:
    void readyRead();
    void disconnected();
};

#endif // CLIENT_H
