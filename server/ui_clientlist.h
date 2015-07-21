/********************************************************************************
** Form generated from reading UI file 'clientlist.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTLIST_H
#define UI_CLIENTLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientList
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *serverLable;
    QTableView *clientList;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ClientList)
    {
        if (ClientList->objectName().isEmpty())
            ClientList->setObjectName(QStringLiteral("ClientList"));
        ClientList->resize(369, 355);
        centralwidget = new QWidget(ClientList);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 100, 141, 17));
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 20, 341, 51));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        serverLable = new QLabel(gridLayoutWidget);
        serverLable->setObjectName(QStringLiteral("serverLable"));
        serverLable->setAlignment(Qt::AlignCenter);
        serverLable->setWordWrap(true);

        gridLayout->addWidget(serverLable, 0, 0, 1, 1);

        clientList = new QTableView(centralwidget);
        clientList->setObjectName(QStringLiteral("clientList"));
        clientList->setGeometry(QRect(10, 120, 256, 192));
        ClientList->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ClientList);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 369, 23));
        ClientList->setMenuBar(menubar);
        statusbar = new QStatusBar(ClientList);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        ClientList->setStatusBar(statusbar);

        retranslateUi(ClientList);

        QMetaObject::connectSlotsByName(ClientList);
    } // setupUi

    void retranslateUi(QMainWindow *ClientList)
    {
        ClientList->setWindowTitle(QApplication::translate("ClientList", "Connected clients list", 0));
        label->setText(QApplication::translate("ClientList", "Clients connected:", 0));
        serverLable->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ClientList: public Ui_ClientList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTLIST_H
