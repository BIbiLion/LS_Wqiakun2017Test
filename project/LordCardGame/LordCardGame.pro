
QT += gui core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
    RC_FILE = LordCardGame.rc
}

DESTDIR = ./bin
OBJECTS_DIR = ./bin/obj


FORMS += 	MainFrame.ui \
			MarkBoard.ui \
			
RESOURCES += LordCardGame.qrc
			
HEADERS += 	CallLordThread.h \
			CardPic.h \
			Cards.h \
			GameControl.h \
			GamePanel.h \
			Hand.h \
			MainFrame.h \
			MarkBoard.h \
			Player.h \
			PlayHandThread.h \
			Robot.h \
			Strategy.h \
			UserPlayer.h
			
			
SOURCES += 	CallLordThread.cpp \
			CardPic.cpp \
			Cards.cpp \
			GameControl.cpp \
			GamePanel.cpp \
			Hand.cpp \
			MainFrame.cpp \
			MarkBoard.cpp \
			Player.cpp \
			PlayHandThread.cpp \
			Robot.cpp \
			Strategy.cpp \
			UserPlayer.cpp \
			main.cpp
			
