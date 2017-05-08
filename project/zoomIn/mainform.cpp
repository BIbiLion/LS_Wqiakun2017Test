#include "mainform.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <qDebug>
#include <QMouseEvent>
#include <QAction>
#include <QDateTime>
#include <QColor>
#include <QRgb>
/*
声明:此例子为网上项目具体链接找不到了，
后由QQ:419087137 上海-叶海
--修改时间2017年1月2日22:09:50
--修改添加取色功能.
--测试编译环境qt5.4.0 mingw,qt5.6.0 mingw
*/
MainForm::MainForm(QWidget *parent)
    : QWidget(parent),
      _shapeMode(0)
{
    m_pos = QPoint();
    this->resize(200,200);
    setWindowOpacity(1);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QAction *act0,*act1, *act2, *act3,*act4,*act5,*act6,*act7;
    act0 = new QAction("100%", this);
    act1 = new QAction("150%", this);
    act2 = new QAction("200%", this);
    act3 = new QAction("400%", this);
    act4 = new QAction("截取区域图", this);
    act5 = new QAction("截取桌面图", this);
    act6 = new QAction("矩型",this);
    act7 = new QAction("exit", this);
    //多用group
    QActionGroup *group = new QActionGroup(this);
    connect(group,SIGNAL(triggered(QAction*)),
            this,SLOT(slotTriggered( QAction*)));
    group->addAction(act0);
    group->addAction(act1);
    group->addAction(act2);
    group->addAction(act3);
    group->addAction(act4);
    group->addAction(act5);
    group->addAction(act6);
    group->addAction(act7);
    //互斥
    group->setExclusive(true);
    for(int i=0;i<group->actions().size();++i)
    {
        group->actions().at(i)->setData(i);
        group->actions().at(i)->setCheckable(true);
    }

    this->addActions(group->actions());
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    this->installEventFilter(this);
    act0->trigger();

}

MainForm::~MainForm()
{
}

void MainForm::showEvent(QShowEvent *e)
{
    m_pix = QApplication::primaryScreen()->grabWindow(0);
    QWidget::showEvent(e);
}

bool MainForm::eventFilter(QObject *watched, QEvent *event)
{
    if( watched == this )
    {
        if(!m_isDis && QEvent::WindowDeactivate == event->type())
            m_isDis = true;
        else if(m_isDis && QEvent::WindowActivate == event->type())
        {
            this->setVisible(false);//隐藏下自己，防止下面截屏捕捉到
            m_pix = QApplication::primaryScreen()->grabWindow(0);
            this->setVisible(true);

            m_isDis = false;
            return true;
        }
    }
    return false;
}
void MainForm::leaveEvent(QEvent *e)
{
    QWidget::leaveEvent(e);
}
void MainForm::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        m_pos = e->pos();
}

void MainForm::mouseMoveEvent(QMouseEvent *e)
{
    if (m_pos.isNull())
        return;
    if ((e->pos()-m_pos).manhattanLength() > 0) {
        QPoint p = pos()+e->pos()-m_pos;
        if (p.x() <= -(this->width()/2))
            p.setX(-(this->width()/2));
        if (p.x() > m_pix.width()-this->width()/2)
            p.setX(m_pix.width()-this->width()/2);
        if (p.y() <= -(this->height()/2))
            p.setY(-(this->height()/2));
        if (p.y() > m_pix.height()-this->height()/2)
            p.setY(m_pix.height()-this->height()/2);
        this->move(p);
        this->update();
    }
    QWidget::mouseMoveEvent(e);
}

void MainForm::mouseReleaseEvent(QMouseEvent *e)
{
    m_pos = QPoint();
    QWidget::mouseReleaseEvent(e);
}

void MainForm::paintEvent(QPaintEvent *)
{
    QPoint p = pos() + QPoint(m_O,m_O);
    QPainter paint(this);
    QPainterPath path;
    QRect r = this->rect();

    //去锯齿
    paint.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //调整

    if(0==_shapeMode){
        r.adjust(1,1,-1,-1);
        //圆
        path.addEllipse(r);
        m_tempPix = m_pix.copy(QRect(p.x(), p.y(), m_width, m_height))
                .scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    }else if(1==_shapeMode){
        r.adjust(1,1,-1,-1);
        //矩形
        path.addRect(r);
        m_tempPix = m_pix.copy(QRect(p.x(), p.y(), m_width, m_height))
                .scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }


    paint.fillPath(path,
                   QBrush(m_tempPix));
    paint.setPen(Qt::yellow);
    paint.drawPath(path);
    paint.setPen(Qt::blue);
    paint.drawLine(QPointF(0,this->rect().height()/2),
                   QPointF(this->rect().width(),this->rect().height()/2));
    paint.drawLine(QPointF(this->rect().width()/2,0),
                   QPointF(this->rect().width()/2,this->rect().height()));
    QPoint centerPoint = QPoint(this->rect().width()/2,this->rect().height()/2);

    if(!m_tempPix.isNull()){
        QColor color;
        QRgb rgb;
        QString rgbStr ;
# if(QT_VERSION >= QT_VERSION_CHECK(5,6,0))
        {
            color = m_tempPix.toImage().pixelColor(centerPoint);
            rgbStr  = QString("%1,%2,%3,%4")
                    .arg(color.red())
                    .arg(color.green())
                    .arg(color.blue())
                    .arg(color.alpha());
        }
#else
        {
            rgb = m_tempPix.toImage().pixel(centerPoint);
            rgbStr  = QString("%1,%2,%3,%4")
                    .arg(qRed(rgb))
                    .arg(qGreen(rgb))
                    .arg(qBlue(rgb))
                    .arg(qAlpha(rgb));
        }
#endif
        QFont f1("Helvetica [Cronyx]");
        QFontMetrics fm(f1);
        paint.setPen(Qt::red);
        paint.setFont(f1);
        int textWidthInPixels = fm.width(rgbStr);
        int textHeightInPixels = fm.height();
        QRectF textRect = QRectF(centerPoint.x(),centerPoint.y(),textWidthInPixels,textHeightInPixels);
        paint.drawText(textRect,rgbStr);
    }
}
void MainForm::slotTriggered(QAction*ac)
{
    switch (ac->data().toInt()) {
    case 0:
        m_O = 0, m_width = this->width(),m_height = this->height();
        goto update;
        break;
    case 1:
        m_O = 25, m_width = this->width()*0.75; m_height = this->height()*0.75;
        goto update;
        break;
    case 2://自己计算比例
        m_O = 50, m_width = m_height = 100;
        goto update;
        break;
    case 3:
        m_O = 75, m_width = m_height = 50;
        goto update;
        break;
    case 4:
        m_tempPix.save(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz.png"));
        goto update;
        break;
    case 5:
        m_pix.save(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz.png"));
        goto update;
        break;
    case 6:
        if(0==_shapeMode){
            ac->setText("圆形");
            _shapeMode = 1;
        }else{
            ac->setText("矩形");
            _shapeMode = 0;
        }
        goto update;
        break;
    case 7:
        QApplication::quit();
        break;
    default:
        break;
    }
    return;
update:
    {
        this->update();
    }
}

