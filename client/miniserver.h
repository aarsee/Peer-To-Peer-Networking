#ifndef MINISERVER_H
#define MINISERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QList>
#include <QFile>
#include <QMap>
#include <QCryptographicHash>

typedef struct socketData socketData;

class MiniServer : public QObject
{
    Q_OBJECT
public:
    explicit MiniServer(QString filename, QObject *parent = 0);
    QString initialize();
    void parseFile(QString filename);
    QList<socketData *> socDataList;
    QList<QTcpSocket *>clientList;
    QList<QString> pieceHashes;
    QString serverDetails;
    QString tfilename;
    QString filename;
    qint64 sizechk;
    qint64 size;
    qint64 pieceSize;
    qint64 numberOfBytesWritten;
    QMap<qint64, bool> map;

signals:
    void dataGot();
    void fileTransferDone();

public slots:
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpServer *server;
};

#endif // MINISERVER_H
