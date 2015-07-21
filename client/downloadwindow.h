#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include <QMainWindow>
#include <client.h>

namespace Ui {
class DownloadWindow;
}

class DownloadWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DownloadWindow(client *client, QWidget *parent = 0);
    ~DownloadWindow();

private slots:
    void on_browse_clicked();
    void on_start_downloading_clicked();
    void valueChanged();
    void fileTransferDone();

private:
    Ui::DownloadWindow *ui;
    QString filename;
    client *clnt;
};

#endif // DOWNLOADWINDOW_H
