#-------------------------------------------------
#
# Project created by QtCreator 2016-12-26T15:20:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = freamWork_configuration
TEMPLATE = app

LIBS+= "E:\\LSwuqiankun\\LSWuqiankun\\freamWork_configuration\\debug\\testOriganize.dll"
#LIBS+= "E:\\LSwuqiankun\\LSWuqiankun\\freamWork_configuration\\debug\\C_sharedDll.dll"
LIBS+= "E:\LSwuqiankun\LSWuqiankun\freamWork_configuration\C_sharedDllBase.dll"
SOURCES += main.cpp\
        mainwindow.cpp \
    form.cpp \
    toolbutton.cpp \
    testwidget.cpp \
    simpleitem.cpp \
    mygraphicsview.cpp \
    myscene.cpp

HEADERS  += mainwindow.h \
    form.h \
    toolbutton.h \
    testwidget.h \
    simpleitem.h \
    mygraphicsview.h \
    myscene.h

FORMS    += mainwindow.ui \
    form.ui

RESOURCES += \
    rc.qrc
