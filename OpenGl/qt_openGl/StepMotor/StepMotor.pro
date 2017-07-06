#-------------------------------------------------
#
# Project created by QtCreator 2015-08-26T08:24:43
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport
QT       +=opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StepMotor
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    GLWidget.cpp

HEADERS  += MainWindow.h \
    GLWidget.h

FORMS    += MainWindow.ui

RESOURCES += \
    picture.qrc
