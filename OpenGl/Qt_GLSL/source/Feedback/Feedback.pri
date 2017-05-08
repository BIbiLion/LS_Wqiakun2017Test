# 有关Feedback模块

CONFIG( feedback )
{
        QT += network
        HEADERS += Feedback/FeedbackDialog.h
	LIBS += $${_PRO_FILE_PWD_}/lib/libFeedback.a
	RESOURCES += Feedback/Feedback.qrc
}
