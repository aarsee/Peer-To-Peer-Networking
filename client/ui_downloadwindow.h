/********************************************************************************
** Form generated from reading UI file 'downloadwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOADWINDOW_H
#define UI_DOWNLOADWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DownloadWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *lineEdit;
    QPushButton *browse;
    QPushButton *start_downloading;
    QProgressBar *downloadBar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DownloadWindow)
    {
        if (DownloadWindow->objectName().isEmpty())
            DownloadWindow->setObjectName(QStringLiteral("DownloadWindow"));
        DownloadWindow->resize(454, 338);
        centralwidget = new QWidget(DownloadWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(0, 50, 371, 29));
        browse = new QPushButton(centralwidget);
        browse->setObjectName(QStringLiteral("browse"));
        browse->setGeometry(QRect(380, 50, 71, 27));
        start_downloading = new QPushButton(centralwidget);
        start_downloading->setObjectName(QStringLiteral("start_downloading"));
        start_downloading->setGeometry(QRect(130, 150, 151, 27));
        downloadBar = new QProgressBar(centralwidget);
        downloadBar->setObjectName(QStringLiteral("downloadBar"));
        downloadBar->setGeometry(QRect(17, 230, 421, 23));
        downloadBar->setValue(24);
        DownloadWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DownloadWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 454, 23));
        DownloadWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(DownloadWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        DownloadWindow->setStatusBar(statusbar);

        retranslateUi(DownloadWindow);

        QMetaObject::connectSlotsByName(DownloadWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DownloadWindow)
    {
        DownloadWindow->setWindowTitle(QApplication::translate("DownloadWindow", "Download window", 0));
        browse->setText(QApplication::translate("DownloadWindow", "Browse", 0));
        start_downloading->setText(QApplication::translate("DownloadWindow", "Start Downloading", 0));
    } // retranslateUi

};

namespace Ui {
    class DownloadWindow: public Ui_DownloadWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOADWINDOW_H
