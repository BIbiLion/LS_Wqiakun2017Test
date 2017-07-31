#-------------------------------------------------
#
# Project created by QtCreator 2014-06-11T14:07:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChartDierctorTest2
TEMPLATE = app
DEFINES += CHARTDIR_HIDE_OBSOLETE _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += E:\LSwuqiankun\ChartDierctorTest2\ChartDirector\include
LIBS += E:\LSwuqiankun\ChartDierctorTest2\ChartDirector\lib\chartdir50.lib

SOURCES +=\
        widget.cpp \
    main.cpp \
    qchartviewer.cpp \
    cdefinechart.cpp \
    cpiechart.cpp \
    chistogram.cpp \
    clinechart.cpp \
    config.cpp \
    meterwidget.cpp \
    realtimedemo.cpp \
    xylinewidget.cpp \
    clockqt.cpp

HEADERS  += widget.h \
    qchartviewer.h \
    cdefinechart.h \
    cpiechart.h \
    chistogram.h \
    clinechart.h \
    config.h \
    meterwidget.h \
    realtimedemo.h \
    xylinewidget.h \
    clockqt.h

FORMS    += widget.ui

OTHER_FILES += \
    CDML.txt
