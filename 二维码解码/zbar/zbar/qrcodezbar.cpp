#include "qrcodezbar.h"
#include "ui_qzbar.h"
#include "scanimage.h"
#ifdef QRDECODE
    #include "scanimagemagick.h"
#endif

QRCodeZbar::QRCodeZbar(QWidget *parent)
    : QWidget(parent, Qt::Dialog)
    , ui(new Ui::QZbar)
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    ui->setupUi(this);
}

QRCodeZbar::~QRCodeZbar()
{

}

void QRCodeZbar::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("choose a picture"),
                                                    QApplication::applicationDirPath(),
                                                    tr("all Files (*.*)"));
    ui->lineEdit->setText(fileName);
    ui->label_3->setPixmap(QPixmap(ui->lineEdit->text()));
    char result[1024] = {0};
    #ifdef QRDECODE
        scanimagemagick(ui->lineEdit->text().toStdString().data(), result);
    #else
        scanimage(ui->lineEdit->text().toStdString().data(), result);
    #endif
    ui->textEdit->setText(result);
}
