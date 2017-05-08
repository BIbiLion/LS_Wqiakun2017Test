#include "MainFrame.h"

MainFrame::MainFrame(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);

	m_gamePanel = new GamePanel(this);

	setFixedSize(1000, 650);
	setWindowTitle(tr("Lord Card Game"));
}

MainFrame::~MainFrame()
{

}

void MainFrame::resizeEvent(QResizeEvent* event)
{
	m_gamePanel->setGeometry(rect());

	QDialog::resizeEvent(event);
}
