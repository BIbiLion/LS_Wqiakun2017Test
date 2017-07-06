#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T18:18:25
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = htmlJs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mywebkit.cpp \
    qwebkittest_test.cpp

HEADERS  += mainwindow.h \
    mywebkit.h \
    qwebkittest_test.h

FORMS    += mainwindow.ui \
    qwebkittest_test.ui

RESOURCES +=
