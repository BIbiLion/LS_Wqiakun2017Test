#include "qrcondezxing.h"
#include "ui_qrcondezxing.h"
#include "QZXing.h"

QRCondeZxing::QRCondeZxing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRCondeZxing)
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(codec);
//    QTextCodec::setCodecForCStrings(codec);
//    QTextCodec::setCodecForTr(codec);
    ui->setupUi(this);
}

QRCondeZxing::~QRCondeZxing()
{
    delete ui;
}

void QRCondeZxing::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("choose a picture"),
                                                    QApplication::applicationDirPath(),
                                                    tr("all Files (*.*)"));
    ui->lineEdit->setText(fileName);
    ui->label_3->setPixmap(QPixmap(ui->lineEdit->text()));
    QZXing decoder;
    QString qrmsg = decoder.decodeImageFromFile(ui->lineEdit->text());
    ui->textEdit->setText(qrmsg);
}
