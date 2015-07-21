/********************************************************************************
** Form generated from reading UI file 'keywindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYWINDOW_H
#define UI_KEYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KeyWindow
{
public:
    QWidget *centralwidget;
    QPushButton *download;
    QPushButton *create_torrent;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *KeyWindow)
    {
        if (KeyWindow->objectName().isEmpty())
            KeyWindow->setObjectName(QStringLiteral("KeyWindow"));
        KeyWindow->resize(454, 352);
        centralwidget = new QWidget(KeyWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        download = new QPushButton(centralwidget);
        download->setObjectName(QStringLiteral("download"));
        download->setGeometry(QRect(100, 170, 101, 27));
        create_torrent = new QPushButton(centralwidget);
        create_torrent->setObjectName(QStringLiteral("create_torrent"));
        create_torrent->setGeometry(QRect(250, 170, 111, 27));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 30, 411, 20));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);
        KeyWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(KeyWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 454, 23));
        KeyWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(KeyWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        KeyWindow->setStatusBar(statusbar);

        retranslateUi(KeyWindow);

        QMetaObject::connectSlotsByName(KeyWindow);
    } // setupUi

    void retranslateUi(QMainWindow *KeyWindow)
    {
        KeyWindow->setWindowTitle(QApplication::translate("KeyWindow", "Download or create torrent", 0));
        download->setText(QApplication::translate("KeyWindow", "Download", 0));
        create_torrent->setText(QApplication::translate("KeyWindow", "Create Torrent", 0));
        label->setText(QApplication::translate("KeyWindow", "You're now connected to the central server.", 0));
    } // retranslateUi

};

namespace Ui {
    class KeyWindow: public Ui_KeyWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYWINDOW_H
