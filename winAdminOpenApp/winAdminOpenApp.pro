#-------------------------------------------------
#
# Project created by QtCreator 2017-06-16T10:57:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = winAdminOpenApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    manifest.xml

 message("this is a test message")
 warning("Unknown configuration")
 message($$PWD)
 message($$OUT_PWD)
 message($$TARGET)

 message($$escape_expand)
win32{
 # QMAKE_POST_LINK += $$OUT_PWD/debug/mt.exe -manifest $$PWD/manifest.xml -outputresource:$$OUT_PWD/debug/$$TARGET".exe" #$$escape_expand(\n\t)
  #message($$OUT_PWD/debug/$$TARGET".exe")#output path

 QMAKE_LFLAGS += /MANIFESTUAC:\”level=\'requireAdministrator\'uiAccess=\'false\' \”
}

