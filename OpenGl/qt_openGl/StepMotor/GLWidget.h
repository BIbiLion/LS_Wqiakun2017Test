#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <gl/glu.h>
#include <math.h>
#include <QDebug>
#include <QPoint>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QDataStream>
#include <QIODevice>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QThread>
#include <QLabel>
#include <QTime>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w,int h);
    void paintGL();
    void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,GLint teeth, GLfloat tooth_depth);
    void paintCylinder(GLdouble size[],GLfloat body_color[],GLfloat color[],GLfloat position[]);

    void keyPressEvent(QKeyEvent* e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    void initVariable();

    ~GLWidget();

    GLfloat x,y,z,translate;
    GLint gear1;
    GLUquadricObj *quar;
    QPoint lastPos;
    QSerialPort com;
    QTimer timer;
    QString strComName;
    QByteArray qbya;
    QString strToHex;
    QString strFindCom;
    qint8 outChar;
    uint portData[20];
    uint lastData;
    uint writePortData;
    int j,countQbyte;
    int countGearSpin;
    //int comNum;
    bool flagPortData;
    QLabel lab;

signals:
    void sigDisplayLed6to0(uint);
    void sigDisplayLed7(uint);
    void sigFullScreen();
    void sigQuitFullScreen();

public slots:
    void startGearSpin();
    void findCom();

};

#endif // GLWIDGET_H
