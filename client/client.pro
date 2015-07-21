#-------------------------------------------------
#
# Project created by QtCreator 2014-09-25T18:37:42
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    miniserver.cpp \
    client.cpp \
    keywindow.cpp \
    downloadwindow.cpp \
    createtorrentwindow.cpp

HEADERS  += mainwindow.h \
    miniserver.h \
    client.h \
    keywindow.h \
    downloadwindow.h \
    createtorrentwindow.h

FORMS    += mainwindow.ui \
    keywindow.ui \
    downloadwindow.ui \
    createtorrentwindow.ui
