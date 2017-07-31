#-------------------------------------------------
#
# Project created by QtCreator 2016-03-18T13:51:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Display
TEMPLATE = app


SOURCES += main.cpp\
        display.cpp \
    form_km.cpp

HEADERS  += display.h \
    form_km.h

FORMS    += display.ui \
    form_km.ui

RESOURCES += \
    image.qrc
