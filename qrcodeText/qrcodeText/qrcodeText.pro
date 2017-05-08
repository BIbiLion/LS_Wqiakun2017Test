#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T11:11:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qrcodeText
TEMPLATE = app

LIBS+= qrcodelib.lib
LIBS+= qrcodelib.dll

SOURCES += main.cpp\
        mainwindow.cpp \
    qrcodegenerate.cpp

HEADERS  += mainwindow.h \
    qrcodegenerate.h

FORMS    += mainwindow.ui \
    qrcodegenerate.ui
