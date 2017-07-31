#-------------------------------------------------
#
# Project created by QtCreator 2016-08-10T17:58:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = testQCustomplot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    writefile.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    writefile.h

FORMS    += mainwindow.ui
