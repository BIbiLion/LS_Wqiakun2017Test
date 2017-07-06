#-------------------------------------------------
#
# Project created by QtCreator 2014-11-22T18:32:27
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtglModel
TEMPLATE = app


SOURCES += main.cpp\
        glwidget.cpp \
    files/Load3DS.cpp \
    files/LoadOBJ.cpp \
    MeshModel.cpp

HEADERS  += glwidget.h \
    files/Load3DS.h \
    files/LoadOBJ.h \
    MeshModel.h

FORMS    += glwidget.ui

RESOURCES +=
