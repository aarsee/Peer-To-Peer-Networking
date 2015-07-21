#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QList>
#include <QFile>
#include <QCryptographicHash>

typedef struct PeerList PeerList;
typedef struct DownloaderList DownloaderList;

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    QString initialize();
    QList<QTcpSocket *> clientList;
    QList<PeerList *> databasePeers;
    QList<DownloaderList *> databaseDownloaders;
    void emitChangeList();
    QString getIp(QString data);
    bool commandReading;
    QByteArray command;
    bool ipReading;
    bool portReading;
    bool filenameReading;
    QByteArray ip;
    QByteArray port;
    QByteArray filename;

signals:
    void clientChanged();

public slots:
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpServer *server;
};

#endif // MYTCPSERVER_H
