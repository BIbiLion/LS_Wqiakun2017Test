 # ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 # ┃     ┏━┣━┣┓　　┏┓┏┓┳┓┏━━┓┣┣━┓　┓　┓┣┳━┓       ┃
 # ┃     ┏┏┏╯━┓┳┳━┛┏╯┃┃┃　　┃┣┣━┓┃┃　┃┃┃　　       ┃
 # ┃     ┃┃┏━╮┃┗┗┏╯┗┃┃╯┃　　┃┏┣━┓┃┃　┃╯┣━┓       ┃
 # ┃     ╰┫┏━┻╯┗┳┣┛┏┛┃┃┣━━┫┃┃　┃┃┃┗╯　┃　　       ┃
 # ┃     ┏┫━┳━┫┏┃┣┓┗┃┃╯┃　　┃┃┃　┃　┃　┃　┣━┓       ┃
 # ┃     ┗┗┗━━╯┗┛┛╯┗╯╰　┗━━╯　┛　┛┗╯　╰┛┗　　       ┃
 # ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 # ┃                     Copyright (c) 2013 jiangcaiyang                    ┃
 # ┃ This software is provided 'as-is', without any express or implied      ┃
 # ┃ warranty. In no event will the authors be held liable for any damages  ┃
 # ┃ arising from the use of this software.                                 ┃
 # ┃                                                                        ┃
 # ┃ Permission is granted to anyone to use this software for any purpose,  ┃
 # ┃ including commercial applications, and to alter it and redistribute it ┃
 # ┃ freely, subject to the following restrictions:                         ┃
 # ┃                                                                        ┃
 # ┃ 1. The origin of this software must not be misrepresented; you must    ┃
 # ┃    not claim that you wrote the original software. If you use this     ┃
 # ┃    software in a product, an acknowledgment in the product             ┃
 # ┃    documentation would be appreciated but is not required.             ┃
 # ┃ 2. Altered source versions must be plainly marked as such, and must    ┃
 # ┃    not be misrepresented as being the original software.               ┃
 # ┃ 3. This notice may not be removed or altered from any source           ┃
 # ┃    distribution.                                                       ┃
 # ┃                                                                        ┃
 # ┃ jiangcaiyang jiangcaiyang123@163.com                                   ┃
 # ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 # ┃ file name: QtShader_3.pro                                              ┃
 # ┃ create date: 2013年8月24日星期六 22时54分40秒                          ┃
 # ┃ last modified date: 2013年8月24日星期六 22时54分40秒                   ┃
 # ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 # 
TARGET = QtShader

QT += core gui widgets opengl

include ( ShaderEditor/ShaderEditor.pri )
include ( Feedback/Feedback.pri )

OTHER_FILES += \
    DefaultShader.vert \
    DefaultShader.frag

HEADERS += \
    MainWindow.h \
    GLWidget.h \
    Camera.h \
    ShaderDialog.h

SOURCES += \
    MainWindow.cpp \
    main.cpp \
    GLWidget.cpp \
    Camera.cpp \
    ShaderDialog.cpp

RESOURCES += \
    Shader.qrc \
    Icon.qrc \
    Translation.qrc

FORMS += \
    ShaderDialog.ui

TRANSLATIONS += zh_CN.ts
RC_ICONS += appIcon.ico

VERSION = 1.0.0.0
