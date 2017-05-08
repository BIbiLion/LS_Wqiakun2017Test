#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QtGui/QDialog>
#include <QVector>
#include "ui_MainFrame.h"
#include "GamePanel.h"
#include <QResizeEvent>

class MainFrame : public QDialog
{
	Q_OBJECT

public:
	MainFrame(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainFrame();

protected:
	virtual void resizeEvent(QResizeEvent* event);

private:
	Ui::MainFrameClass ui;
	GamePanel* m_gamePanel;
};

#endif // MAINFRAME_H
