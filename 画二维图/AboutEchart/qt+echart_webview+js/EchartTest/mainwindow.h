#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWebView>

class EChartTest;

class MainWin : public QWebView
{
    Q_OBJECT
    
public:
    explicit MainWin(QWidget * parent = 0);

private:
	EChartTest * m_analyzer;

private slots:
    void addJSObject();

};
#endif
