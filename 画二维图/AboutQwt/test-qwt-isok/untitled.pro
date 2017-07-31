#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T15:35:19
#
#-------------------------------------------------

QT       += core gui

CONFIG+=qwt  ##配置信息 CONFIG用来告诉qmake关于应用程序的配置信息。
#DEFINES +=QT_DLL QWT_DLL   # DEFINES += XX_XX_XXX  //定义编译选项，在.h文件中就可以使用 ：#ifdefine xx_xx_xxx
LIBS += -L”C:\Qt\Qt5.2.1\5.2.1\mingw48_32\lib” -lqwtd
LIBS += -L”C:\Qt\Qt5.2.1\5.2.1\mingw48_32\lib” -lqwt

INCLUDEPATH += "C:\Qt\Qt5.2.1\5.2.1\mingw48_32\include\qwt"


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app



SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

