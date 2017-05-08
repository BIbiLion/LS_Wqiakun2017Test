#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ftpmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void S_updateProgess(int);
    void S_updateProgess2(int);

    void on_downloadBn_clicked();

    void on_uploadBn_clicked();

    void on_downloadBn_2_clicked();

    void on_abort_clicked();

    void on_abort_2_clicked();

    void on_abort_3_clicked();

    void on_downloadBn_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    FtpManager *manager;
    FtpManager *manager2;
};

#endif // MAINWINDOW_H
