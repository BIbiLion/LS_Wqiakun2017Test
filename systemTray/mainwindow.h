#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void systemTrayIconActived(QSystemTrayIcon::ActivationReason);

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_systemTrayIcon;
    QAction * m_quitAction;
    QMenu * m_menu;
};

#endif // MAINWINDOW_H
