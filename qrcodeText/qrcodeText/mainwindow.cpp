#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    qr = NULL;

}

MainWindow::~MainWindow()
{
//    if(qr != NULL)
//    {
//       QRcode_free(qr);
//    }
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->widget->generateString(ui->lineEdit->text());
    //setString(ui->lineEdit->text());
}
//void MainWindow::setString(QString str)
//{
//    string = str;
//    if(qr != NULL)
//    {
//        QRcode_free(qr);
//    }
//    qr = QRcode_encodeString(string.toStdString().c_str(),
//                             1,
//                             QR_ECLEVEL_L,
//                             QR_MODE_8,
//                             1);
//    update();

//}
//保存
void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
    if( !fileName.isEmpty() )
    {
        ui->widget->saveImage(fileName);
    }
    //saveImage("d:/tead.png", 250);
}
//bool MainWindow::saveImage(QString fileName, int size)
//{
//    if(size != 0 && !fileName.isEmpty())
//    {
//        QImage image(size, size, QImage::Format_Mono);
//        QPainter painter(&image);
//        QColor background(Qt::white);
//        painter.setBrush(background);
//        painter.setPen(Qt::NoPen);
//        painter.drawRect(0, 0, size, size);
//        if(qr != NULL)
//        {
//            draw(painter, size, size);
//        }
//        return image.save(fileName);
//    }
//    else
//    {
//        return false;
//    }
//}

//void MainWindow::draw(QPainter &painter, int width, int height)
//{
//    QColor foreground(Qt::black);
//    painter.setBrush(foreground);
//    const int qr_width = qr->width > 0 ? qr->width : 1;
//    double scale_x = width / qr_width;
//    double scale_y = height / qr_width;
//    for( int y = 0; y < qr_width; y ++)
//    {
//        for(int x = 0; x < qr_width; x++)
//        {
//            unsigned char b = qr->data[y * qr_width + x];
//            if(b & 0x01)
//            {
//                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
//                painter.drawRects(&r, 1);
//            }
//        }
//    }
//}

//void MainWindow::paintEvent(QPaintEvent *)
//{
//    QPainter painter(this);
//    QColor background(Qt::white);
//    painter.setBrush(background);
//    painter.setPen(Qt::NoPen);
//    painter.drawRect(0, 0, 250, 250);
//    if(qr != NULL)
//    {
//        draw(painter, 250, 250);
//    }
//}

//QSize MainWindow::sizeHint()  const
//{
//    QSize s;
//    if(qr != NULL)
//    {
//        int qr_width = qr->width > 0 ? qr->width : 1;
//        s = QSize(qr_width * 4, qr_width * 4);
//    }
//    else
//    {
//        s = QSize(50, 50);
//    }
//    return s;
//}

//QSize MainWindow::minimumSizeHint()  const
//{
//    QSize s;
//    if(qr != NULL)
//    {
//        int qr_width = qr->width > 0 ? qr->width : 1;
//        s = QSize(qr_width, qr_width);
//    }
//    else
//    {
//        s = QSize(50, 50);
//    }
//    return s;
//}
