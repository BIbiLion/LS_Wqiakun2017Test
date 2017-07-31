#-------------------------------------------------
#
# Project created by QtCreator 2017-02-21T11:29:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = systemTray
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    testobject.cpp \
    TestObjectFunction.cpp

HEADERS  += mainwindow.h \
    testobject.h

FORMS    += mainwindow.ui

RESOURCES += \
    rs.qrc
