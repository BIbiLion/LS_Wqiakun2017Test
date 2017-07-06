#-------------------------------------------------
#
# Project created by QtCreator 2015-10-22T19:35:09
#
#-------------------------------------------------

QT       += core gui
QT  +=opengl
QT  +=network
QT  +=multimedia
QT  += script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagicCube
TEMPLATE = app


SOURCES +=\
        MagicCube.cpp \
    sttthread.cpp \
    Cube.cpp

HEADERS  += MagicCube.h \
    sttthread.h \
    Cube.h

FORMS    += MagicCube.ui
