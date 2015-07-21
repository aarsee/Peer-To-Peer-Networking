/********************************************************************************
** Form generated from reading UI file 'createtorrentwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATETORRENTWINDOW_H
#define UI_CREATETORRENTWINDOW_H

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

class Ui_CreateTorrentWindow
{
public:
    QWidget *centralwidget;
    QPushButton *browse;
    QPushButton *create;
    QLineEdit *lineEdit;
    QProgressBar *progressBar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CreateTorrentWindow)
    {
        if (CreateTorrentWindow->objectName().isEmpty())
            CreateTorrentWindow->setObjectName(QStringLiteral("CreateTorrentWindow"));
        CreateTorrentWindow->resize(456, 334);
        centralwidget = new QWidget(CreateTorrentWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        browse = new QPushButton(centralwidget);
        browse->setObjectName(QStringLiteral("browse"));
        browse->setGeometry(QRect(370, 50, 81, 27));
        create = new QPushButton(centralwidget);
        create->setObjectName(QStringLiteral("create"));
        create->setGeometry(QRect(160, 140, 101, 27));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 50, 351, 29));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(7, 210, 441, 23));
        progressBar->setValue(24);
        CreateTorrentWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CreateTorrentWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 456, 23));
        CreateTorrentWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CreateTorrentWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        CreateTorrentWindow->setStatusBar(statusbar);

        retranslateUi(CreateTorrentWindow);

        QMetaObject::connectSlotsByName(CreateTorrentWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CreateTorrentWindow)
    {
        CreateTorrentWindow->setWindowTitle(QApplication::translate("CreateTorrentWindow", "MainWindow", 0));
        browse->setText(QApplication::translate("CreateTorrentWindow", "Browse", 0));
        create->setText(QApplication::translate("CreateTorrentWindow", "Create", 0));
    } // retranslateUi

};

namespace Ui {
    class CreateTorrentWindow: public Ui_CreateTorrentWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATETORRENTWINDOW_H
