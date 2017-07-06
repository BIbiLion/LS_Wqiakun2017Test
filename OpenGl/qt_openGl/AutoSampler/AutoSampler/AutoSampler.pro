#-------------------------------------------------
#
# Project created by QtCreator 2015-08-29T16:42:35
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport
QT       +=opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoSampler
TEMPLATE = app


SOURCES += main.cpp\
        AutoSampler.cpp \
    glwidget.cpp

HEADERS  += AutoSampler.h \
    glwidget.h

FORMS    += AutoSampler.ui

RESOURCES += \
    picture.qrc
