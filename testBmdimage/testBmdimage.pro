#-------------------------------------------------
#
# Project created by QtCreator 2016-06-06T17:48:12
#
#-------------------------------------------------

QT       += widgets network sql script xml

TARGET = testBmdimage
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH+=../../../workspace/lt_pro_0519/include/ltplugin/

SOURCES += \
    ../../bmdimage/bmdimage.cpp \
    ../../bmdimage/bmdimagemain.cpp \
    ../../bmdimage/bmdsatellite.cpp \
    ../../bmdimage/mercatorsatellite.cpp

HEADERS += testbmdimage.h \
    ../../bmdimage/mercatorsatellite.h \
    ../../bmdimage/bmdimage.h \
    ../../bmdimage/bmdsatellite.h \
    ../../../workspace/lt_pro_0519/include/ltplugin/ltplugin_api.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
