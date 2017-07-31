#-------------------------------------------------
#
# Project created by QtCreator 2017-07-19T10:24:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = skyBox
TEMPLATE = app


SOURCES += \
    main.cpp \
    glwidget.cpp \
    skybox.cpp \
    torus.cpp

HEADERS  += \
    glwidget.h \
    skybox.h \
    torus.h

FORMS    +=

OTHER_FILES += \
    skybox_fragment.glsl \
    skybox_vertex.glsl \
    torus_vertex.glsl \
    torus_fragment.glsl

#qt5.7 输出断言信息
    win32:LIBS += -lOpengl32 \
                    -lglu32 \
                    -lglut
