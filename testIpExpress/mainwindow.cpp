#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QRegExpValidator>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);//

    //((2[0-4]\d|25[0-5]|[01]?\d\d?)\.){3}(2[0-4]\d|25[0-5]|[01]?\d\d?)
    QString pattern("(1\\d{0,2}|2[0-4]\\d?|25[0-5])\\.(0|1\\d{0,2}|2[0-4]\\d?|25[0-5])\.(0|1\\d{0,2}|2[0-4]\\d?|25[0-5])\\.(0|1\\d{0,2}|2[0-4]\\d?|25[0-5])");
    QRegExp ipRegExp = QRegExp(pattern);//("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){4}");
    QRegExpValidator *ipRegExpValidator = new QRegExpValidator(ipRegExp, this);

    ui->lineEdit->setValidator(ipRegExpValidator);
}
//第一位数字   1\\d{0,2}|2[0-4]\\d?|25[0-5]    1-255

MainWindow::~MainWindow()
{
    delete ui;
}
