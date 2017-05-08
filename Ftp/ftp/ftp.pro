#-------------------------------------------------
#
# Project created by QtCreator 2014-04-09T10:24:01
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ftp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qftp.cpp \
    qurlinfo.cpp \
    ftpmanager.cpp

HEADERS  += mainwindow.h \
    qftp.h \
    qurlinfo.h \
    ftpmanager.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ftp.pro.user \
    ../dll/ftp/ftp.pro.user

SUBDIRS += \
    ../dll/ftp/ftp.pro
