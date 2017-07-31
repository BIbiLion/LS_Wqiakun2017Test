#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T19:32:31
#
#-------------------------------------------------

QT       += core gui

CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
LIBS += -L"D:\Qt\Qt5.2.1\5.2.1\mingw48_32\lib" -lqwtd
LIBS += -L"D:\Qt\Qt5.2.1\5.2.1\mingw48_32\lib" -lqwt

INCLUDEPATH += D:\Qt\Qt5.2.1\5.2.1\mingw48_32\include\Qwt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testQwt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
