#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T12:28:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Utf8ToGbk
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

DISTFILES += \
    app.rc

RC_FILE = app.rc

DESTDIR = $$PWD/bin
