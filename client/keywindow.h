#ifndef KEYWINDOW_H
#define KEYWINDOW_H

#include <QMainWindow>
#include <client.h>

namespace Ui {
class KeyWindow;
}

class KeyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit KeyWindow(client *client, QWidget *parent = 0);
    ~KeyWindow();

private slots:
    void on_download_clicked();
    void on_create_torrent_clicked();

private:
    Ui::KeyWindow *ui;
    client *clnt;
};

#endif // KEYWINDOW_H
