#ifndef CREATETORRENTWINDOW_H
#define CREATETORRENTWINDOW_H

#include <QMainWindow>
#include <client.h>

namespace Ui {
class CreateTorrentWindow;
}

class CreateTorrentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateTorrentWindow(client *clnt, QWidget *parent = 0);
    ~CreateTorrentWindow();
    void createFile(QString filename);
    QString getAddress();

private slots:
    void on_browse_clicked();
    void on_create_clicked();

private:
    Ui::CreateTorrentWindow *ui;
    QString filename;
    client *clnt;
};

#endif // CREATETORRENTWINDOW_H
