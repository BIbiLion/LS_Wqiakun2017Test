# -------------------------------------------------
# QT Project File
# -------------------------------------------------
TARGET = qtdemo

TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
} 

SOURCES += qchartviewer.cpp \
    hotspotdialog.cpp \
    qtdemo.cpp \
    democharts.cpp
    
HEADERS += qchartviewer.h \
    hotspotdialog.h \
    qtdemo.h \
    democharts.h

INCLUDEPATH += ../../include

DEFINES += CHARTDIR_HIDE_OBSOLETE _CRT_SECURE_NO_WARNINGS

win32:DESTDIR = $$PWD
win32:LIBS += ../../lib/chartdir60.lib
win32:QMAKE_POST_LINK += copy /Y ..\\..\\lib\\chartdir60.dll \"$$PWD\"

macx:CONFIG -= app_bundle
macx:DESTDIR = $$PWD
macx:LIBS += -L../../lib -lchartdir
macx:QMAKE_POST_LINK += cp ../../lib/libchartdir.6.dylib \"$(DESTDIR)\"

unix:!macx:DESTDIR = $$PWD
unix:!macx:LIBS += -L../../lib -lchartdir
unix:!macx:QMAKE_RPATHDIR += ../../lib

