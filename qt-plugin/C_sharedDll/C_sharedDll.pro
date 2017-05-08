#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T15:49:06
#
#-------------------------------------------------

QT       += widgets qml network opengl sql script svg xml

TARGET = C_sharedDll
TEMPLATE = lib

DEFINES += C_SHAREDDLL_LIBRARY

SOURCES += c_shareddll.cpp

HEADERS += c_shareddll.h\
        c_shareddll_global.h \
    baseClass/baseclass.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    readMe.txt
