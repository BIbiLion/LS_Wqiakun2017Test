#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage  m_pixmap_one("./3.png");//80 105
    QImage m_pixmap_two("./4.png");

    m_image = new QImage(QSize(160,105),QImage::Format_RGB888);
    //qDebug()<<m_image.width()<<m_image.height();
    for(int i=0;i<80;++i)
    {
        for(int j=0;j<105;++j)
        {
            qDebug()<<i<<j;
            m_image->setPixel(i,j,m_pixmap_one.pixel(i,j));
        }
    }

    for(int i=80;i<160;++i)
    {
        for(int j=0;j<105;++j)
        {
            qDebug()<<i<<j;
            m_image->setPixel(i,j,m_pixmap_two.pixel(i-80,j));
        }
    }
   m_image->save("w.png");
    ui->label->setPixmap(QPixmap::fromImage(*m_image));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::merge()
{
    /*
    label->clear();

    //获取待合并图片的大小
    QSize lts = lefttop->image.size();
    QSize lbs = leftbottom->image.size();
    QSize rts = righttop->image.size();
    QSize rbs = rightbottom->image.size();
    int tw = lts.width() + rts.width();
    int bw = lbs.width() + rbs.width();
    int lh = lts.height() + lbs.height();
    int rh = rts.height() + rbs.height();
    //设置合并后图片的大小
    resultImg = QImage(((tw > bw)? tw: bw), ((lh > rh)? lh: rh), QImage::Format_ARGB32);

    if(!resultImg.isNull()){
        painter = new QPainter(&resultImg);
        //在新区域画图
        painter->drawImage(0, 0, lefttop->image);
        painter->drawImage(lts.width(), 0, righttop->image);
        painter->drawImage(0, lts.height(), leftbottom->image);
        painter->drawImage(lbs.width(), lts.height(), rightbottom->image);
        painter->end();
        //根据显示区大小显示合并后的图片
        //注：保存后的图片大小为原待合并图大小的和，而非显示图片的大小
        QImage displayImg = resultImg.scaled(label->width()-1, label->height()-1, Qt::KeepAspectRatio);
        label->setPixmap(QPixmap::fromImage(displayImg));
    }
    */
}

/**
 * 保存合并后的图片
 */
void MainWindow::save(){
    /*
    if(resultImg.isNull())
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("保存图片"), QDir::currentPath(), tr("*.png;;*.jpg)"));

    if (!fileName.isEmpty()) {
        bool saveOK = resultImg.save(fileName);
        if(!saveOK)
            QMessageBox::information(this, tr("错误提示"), tr("无法保存 %1.").arg(fileName));
    }
    */
}
