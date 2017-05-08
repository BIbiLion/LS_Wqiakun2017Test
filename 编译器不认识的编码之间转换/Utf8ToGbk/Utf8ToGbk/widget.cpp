#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 添加编码
    QList<QByteArray> codecs = QTextCodec::availableCodecs();
    QStringList items;
    items << "UTF-8";

    for (int i = 9; i < codecs.size(); i++) {
        items << codecs.at(i);
    }
    ui->comboBoxCodecSource->clear();
    ui->comboBoxCodecDest->clear();

    ui->comboBoxCodecSource->addItems(items);
    ui->comboBoxCodecSource->setCurrentText("GB18030");

    ui->comboBoxCodecDest->addItems(items);
    ui->comboBoxCodecDest->setCurrentText("UTF8");

    m_strTrPath = "";
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnChange_clicked()
{

    QDir dir (m_strTrPath);
    QDirIterator dirIterator(m_strTrPath,
                             QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                             QDirIterator::Subdirectories);

    ui->textBrowser->clear();

    while (dirIterator.hasNext()) {

        QStringList filters;
        filters << "*.cpp" << "*.h";

        dir.setPath(m_strTrPath);
        ui->textBrowser->append(QString("<span style=\" color:#FF0000;\">进入目录:</span> [ %1 ] <br>").arg(m_strTrPath));
        // 转换文件
        foreach (QFileInfo fileInfo, dir.entryInfoList(filters)) {
            // 转换编码
            changeTextcodec(fileInfo.absoluteFilePath());
        }

        m_strTrPath = dirIterator.next();
    }

}

void Widget::changeTextcodec(QString fileName)
{
    QFile fileRead(fileName);
    fileRead.open(QIODevice::ReadOnly);

    QTextStream out(&fileRead);
    out.setCodec(QTextCodec::codecForName(ui->comboBoxCodecSource->currentText().toLatin1()));
    out.setAutoDetectUnicode(true);

    QString strTemp;

    QStringList strContents;

    while (!out.atEnd()) {
        strTemp = out.readLine();
        strContents << strTemp;
    }
    fileRead.close();

    QFile fielWrite(fileName);

    if (!fielWrite.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        ui->labelStatus->setText("文件打开失败!");
        return;
    }

    QTextStream in(&fielWrite);
    in.setCodec(QTextCodec::codecForName(ui->comboBoxCodecDest->currentText().toLatin1()));
    in.setAutoDetectUnicode(true);

    for (int i = 0; i < strContents.size(); i++)
        in << strContents.at(i) << endl;

    fielWrite.close();

    ui->textBrowser->append(QString("    <span style=\" color:#0000FF;\">转换完成:</span> [ %1 ] <br>").arg(fileName));
}

void Widget::on_btnProject_clicked()
{
    static QString strDir = "d:/";
    strDir = QFileDialog::getExistingDirectory(this, tr("选择工程"),
                                               strDir,
                                               QFileDialog::ShowDirsOnly |
                                               QFileDialog::DontResolveSymlinks);
    // 选择工程目录
    if (!strDir.isEmpty()) {
        m_strTrPath = strDir;
        ui->labelStatus->setText(QString("选择目录[%1]").arg(strDir));
    }
    else {
        strDir = "d:/";
    }
}
