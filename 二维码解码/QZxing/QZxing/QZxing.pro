#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T10:23:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QZxing
TEMPLATE = app


SOURCES += main.cpp\
        qrcondezxing.cpp

HEADERS  += qrcondezxing.h

FORMS    += qrcondezxing.ui

include(QZXing_sourceV2.3/QZXing.pri)
