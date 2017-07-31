#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T06:24:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt5OpenGL
TEMPLATE = app


SOURCES += main.cpp \
   window.cpp \
    basicusagescene.cpp \
    openglWidget/glwidget.cpp


HEADERS += \
    window.h \
    abstractscene.h \
    basicusagescene.h \
    glassert.h \
    openglWidget/glwidget.h


OTHER_FILES += \
    Shaders/phong.vert \
    Shaders/phong.frag \
    shaders/phong.vert \
    shaders/phong.frag \
    cub.frag \
    cub.vert \
    skyBox.frag \
    skyBox.vert

RESOURCES += \
    resources.qrc

#qt5.7 输出断言信息
    win32:LIBS += -lOpengl32 \
                    -lglu32 \
                    -lglut
