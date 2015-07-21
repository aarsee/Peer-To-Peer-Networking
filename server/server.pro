#-------------------------------------------------
#
# Project created by QtCreator 2014-09-24T22:14:06
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mytcpserver.cpp \
    clientlist.cpp

HEADERS  += mainwindow.h \
    mytcpserver.h \
    clientlist.h

FORMS    += mainwindow.ui \
    clientlist.ui
