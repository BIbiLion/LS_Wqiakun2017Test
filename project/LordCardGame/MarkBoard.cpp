#include "MarkBoard.h"

MarkBoard::MarkBoard(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);
}

MarkBoard::~MarkBoard()
{

}

void MarkBoard::SetMarks(int me, int left, int right)
{
	QString str = tr("%1 point(s)");

	ui.myMark->setText(str.arg(me));
	ui.leftMark->setText(str.arg(left));
	ui.rightMark->setText(str.arg(right));
}
