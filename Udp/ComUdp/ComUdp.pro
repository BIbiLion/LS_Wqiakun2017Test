#-------------------------------------------------
#
# Project created by QtCreator 2016-06-21T18:00:20
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComUdp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    class/serialattri.cpp \
    Object/serialportobject.cpp

HEADERS  += mainwindow.h \
    class/serialattri.h \
    Object/serialportobject.h

FORMS    += mainwindow.ui
