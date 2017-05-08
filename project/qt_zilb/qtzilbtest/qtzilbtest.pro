#-------------------------------------------------
#
# Project created by QtCreator 2014-08-15T08:49:04
#
#-------------------------------------------------

QT       += core gui  widgets

TARGET = qtzilbtest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    zlib.h \
    zip.h \
    zconf.h \
    unzip.h \
    ioapi.h

FORMS    += mainwindow.ui
LIBS += zlibstat.lib
