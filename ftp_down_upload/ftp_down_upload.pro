#-------------------------------------------------
#
# Project created by QtCreator 2017-01-06T13:16:39
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ftp_down_upload
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qftp.cpp \
    qurlinfo.cpp \
    ftp.cpp \
    thread/threadftp.cpp \
    Widget/widget.cpp \
    mainwidget.cpp

HEADERS  += mainwindow.h \
    qftp.h \
    qurlinfo.h \
    ftp.h \
    threadftp.h \
    thread/threadftp.h \
    Widget/widget.h \
    mainwidget.h

FORMS    += mainwindow.ui \
    Widget/widget.ui \
    mainwidget.ui
