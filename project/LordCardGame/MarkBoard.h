#ifndef MARKBOARD_H
#define MARKBOARD_H

#include <QFrame>
#include "ui_MarkBoard.h"

class MarkBoard : public QFrame
{
	Q_OBJECT

public:
	MarkBoard(QWidget *parent = 0);
	~MarkBoard();

	void SetMarks(int me, int left, int right);

private:
	Ui::MarkBoard ui;
};

#endif // MARKBOARD_H
