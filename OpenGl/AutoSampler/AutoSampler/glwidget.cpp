/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "glwidget.h"

static GLfloat spin=-35.0;
static GLint status=1;

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
       initVariable();
       lab.setText("串口不可用");
       /*connect(&com,SIGNAL(readyRead()),this,SLOT(findCom()));
       int comNum=1;
       while(comNum<=4)
       {
           strComName="COM"+QString::number(comNum);
           findCom();
           comNum++;
       }*/
       strComName="COM2";
       findCom();

}

void GLWidget::initVariable()
{
    gear1=0;
    xRot = 0;
    yRot = 0;
    zRot = 0;
    countDiskSpin=0;
    upDown=0.0;
    samplingUpDown1=0.0;
    samplingUpDown2=0.0;
     j=0;
    countQbyte=0;
    flagPortData=true;
    gear1Rot = 0-(4.8/4.0 * 16.0)*2;
    writePortData=0xFF;
}

void GLWidget::findCom()
{
   /*if(com.bytesAvailable()>0)
    {
        if(com.readAll().toHex()=="ff")
        {
            disconnect(&com,SIGNAL(readyRead()),this,SLOT(findCom()));
            connect(&com,SIGNAL(readyRead()),this,SLOT(advanceGears()),Qt::QueuedConnection);
            lab.setText("找到"+strComName+"可用");
            qDebug()<<lab.text()<<endl;
            return;
        }
    }
    connect(&com,SIGNAL(readyRead()),this,SLOT(advanceGears()),Qt::QueuedConnection);
    com.close();*/
    com.setPortName(strComName);
    if(!com.open(QIODevice::ReadWrite))
    {
        qDebug()<<"打开"<<strComName<<"失败"<<endl;
        return;
    }
    qDebug()<<"OK"<<endl;
    com.setBaudRate(QSerialPort::Baud115200);
    com.setDataBits(QSerialPort::Data8);
    com.setParity(QSerialPort::NoParity);
    com.setStopBits(QSerialPort::OneStop);
    com.setFlowControl(QSerialPort::NoFlowControl);

    //strToHex=QChar(0xFF);
    //com.write(strToHex.toLatin1());
    connect(&com,SIGNAL(readyRead()),this,SLOT(advanceGears()),Qt::QueuedConnection);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    glDeleteLists(gear1, 1);
}

void GLWidget::initializeGL()//执行初始化工作
{
    static const GLfloat lightPos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };
    static const GLfloat reflectance1[4] = { 0.8f, 0.1f, 0.0f, 0.5f };//颜色设置
    static const GLfloat reflectance2[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
    static const GLfloat reflectance3[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
    static const GLfloat reflectance4[4] = { 0.7f, 0.7f, 0.7f, 0.5f };
    static const GLfloat reflectance5[4] = { 0.3f, 0.3f, 0.3f, 0.3f };


    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    gear1 = makeGear(reflectance1, 0.4, 4.0, 1.0, 0.0, 20);//中间孔，齿轮直径，齿轮厚度，尺长，齿轮密集度
    gear3 = makeGear(reflectance3, 0.4, 4.0, 6.0, 0.0, 20);//底座
    gear4 = makeGear(reflectance4, 0.0, 0.8, 10.6, 0.0, 10);//中轴和取样器架子
    gear2 = makeGear(reflectance2, 0.0, 0.15, 4.1, 0.0, 10);//装样品管子
    gear10 = makeGear(reflectance5, 0.0, 0.15, 4.1, 0.0, 10);//装样品管子
    gear5 = makeGear(reflectance5, 0.0, 2.5, 4.05,0.0, 20);//电机
    gear6 = makeGear(reflectance2, 0.8, 1.5,4.0,0.0,20);//支架
    gear7=makeGear(reflectance5,0.0,0.3,3.2,0.0,20);//拐弯
    gear8=makeGear(reflectance2,0.0,0.075,4.0,0.0,20);//取样器
    gear9=makeGear(reflectance3,0.0,3.3,5.0,0.0,20);//后底座
    gear11 = makeGear(reflectance5, 0.1, 0.1, 0.1, 0.0, 10);

    glEnable(GL_NORMALIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
}

void GLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
//-----------------------------------------
    drawGear2(gear3, 0.0, 0.0, -5.0);
    drawGear2(gear5,0.0,0.0,-6.0);
    drawGear2(gear9,0.0,5.0,-5.5);
    drawGear2(gear6,0.0,6.0,-1.0);
    drawGear2(gear4,0.0,6.0,5.5);
//-----------------------------------------
    drawGear4(gear8,0.0,3.6,7.0);
    drawGear5(gear8,0.0,3.0,7.0);
    drawGear6(gear11,0.0,0.0,-2.0);

    glRotated(+90.0, -1.0, 0.0, 0.0);
    drawGear3(gear7,0.0,-8.5,4.0);
    glRotated(-90.0, -1.0, 0.0, 0.0);

    drawGear(gear1, 0.0, 0.0, 2.5, gear1Rot / 16.0);
#if 1
    drawGear(gear1, 0.0, 0.0, -1.5, gear1Rot / 16.0);
    drawGear(gear4,0.0,0.0,-2.75,gear1Rot/16.0);

    /*drawGear(gear2, 0.0, -3.0, 1.0, 0);
    drawGear(gear2, 0.0, -3.6, 1.0, 0);
    drawGear(gear2,2.1213,-2.1213,1.0,0);
    drawGear(gear2,2.5456,-2.5456,1.0,0);
    drawGear(gear2, 3.0, 0.0, 1.0, 0);
    drawGear(gear2, 3.6, 0.0, 1.0, 0);
    drawGear(gear2,2.1213,2.1213,1.0,0);
    drawGear(gear2,2.5456,2.5456,1.0,0);
    drawGear(gear2, 0.0, 3.0, 1.0, 0);
    drawGear(gear10, 0.0, 3.6, 1.0, 0);
    drawGear(gear2,-2.1213,2.1213,1.0,0);
    drawGear(gear2,-2.5456,2.5456,1.0,0);
    drawGear(gear2, -3.0, 0.0, 1.0, 0);
    drawGear(gear2, -3.6, 0.0, 1.0, 0);
    drawGear(gear2,-2.1213,-2.1213,1.0,0);
    drawGear(gear2,-2.5456,-2.5456,1.0,0);

    drawGear(gear2,1.148,2.772,1.0,0);
    drawGear(gear2,1.378,3.326,1.0,0);
    drawGear(gear2,2.772,1.148,1.0,0);
    drawGear(gear2,3.326,1.378,1.0,0);
    drawGear(gear2,-1.148,2.772,1.0,0);
    drawGear(gear2,-1.378,3.326,1.0,0);
    drawGear(gear2,2.772,-1.148,1.0,0);
    drawGear(gear2,3.326,-1.378,1.0,0);
    drawGear(gear2,1.148,-2.772,1.0,0);
    drawGear(gear2,1.378,-3.326,1.0,0);
    drawGear(gear2,-2.772,1.148,1.0,0);
    drawGear(gear2,-3.326,1.378,1.0,0);
    drawGear(gear2,-1.148,-2.772,1.0,0);
    drawGear(gear2,-1.378,-3.326,1.0,0);
    drawGear(gear2,-2.772,-1.148,1.0,0);
    drawGear(gear2,-3.326,-1.378,1.0,0);

    drawGear(gear2,0.585,2.942,1.0,0);
    drawGear(gear2,0.702,3.531,1.0,0);
    drawGear(gear2,1.667,2.494,1.0,0);
    drawGear(gear2,2.000,2.993,1.0,0);
    drawGear(gear2,-0.585,2.942,1.0,0);
    drawGear(gear2,-0.702,3.531,1.0,0);
    drawGear(gear2,1.667,-2.494,1.0,0);
    drawGear(gear2,2.000,-2.993,1.0,0);
    drawGear(gear2,0.585,-2.942,1.0,0);
    drawGear(gear2,0.702,-3.531,1.0,0);
    drawGear(gear2,-1.667,2.494,1.0,0);
    drawGear(gear2,-2.000,2.993,1.0,0);
    drawGear(gear2,-0.585,-2.942,1.0,0);
    drawGear(gear2,-0.702,-3.531,1.0,0);
    drawGear(gear2,-1.667,-2.494,1.0,0);
    drawGear(gear2,-2.000,-2.993,1.0,0);


    drawGear(gear2,2.942,0.585,1.0,0);
    drawGear(gear2,3.531,0.702,1.0,0);
    drawGear(gear2,2.494,1.667,1.0,0);
    drawGear(gear2,2.993,2.000,1.0,0);
    drawGear(gear2,-2.942,0.585,1.0,0);
    drawGear(gear2,-3.531,0.702,1.0,0);
    drawGear(gear2,2.494,-1.667,1.0,0);
    drawGear(gear2,2.993,-2.000,1.0,0);
    drawGear(gear2,2.942,-0.585,1.0,0);
    drawGear(gear2,3.531,-0.702,1.0,0);
    drawGear(gear2,-2.494,1.667,1.0,0);
    drawGear(gear2,-2.993,2.000,1.0,0);
    drawGear(gear2,-2.942,-0.585,1.0,0);
    drawGear(gear2,-3.531,-0.702,1.0,0);
    drawGear(gear2,-2.494,-1.667,1.0,0);
    drawGear(gear2,-2.993,-2.000,1.0,0);*/

    drawGear(gear2,0.0,-3.0,1.0,0);
    drawGear(gear2,0.0,-3.6,1.0,0);
    drawGear(gear2,0.0,3.0,1.0,0);
    drawGear(gear10,0.0,3.6,1.0,0);
    drawGear(gear2,0.375,2.976,1.0,0);
    drawGear(gear2,0.451,3.572,1.0,0);
    drawGear(gear2,0.746,2.906,1.0,0);
    drawGear(gear2,0.895,3.487,1.0,0);
    drawGear(gear2,1.104,2.789,1.0,0);
    drawGear(gear2,1.325,3.347,1.0,0);
    drawGear(gear2,1.445,2.629,1.0,0);
    drawGear(gear2,1.734,3.155,1.0,0);
    drawGear(gear2,1.763,2.427,1.0,0);
    drawGear(gear2,2.116,2.912,1.0,0);
    drawGear(gear2,2.054,2.187,1.0,0);
    drawGear(gear2,2.464,2.624,1.0,0);
    drawGear(gear2,2.312,1.912,1.0,0);
    drawGear(gear2,2.774,2.293,1.0,0);
    drawGear(gear2,2.553,1.607,1.0,0);
    drawGear(gear2,3.040,1.929,1.0,0);
    drawGear(gear2,2.714,1.277,1.0,0);
    drawGear(gear2,3.257,1.533,1.0,0);
    drawGear(gear2,2.853,0.927,1.0,0);
    drawGear(gear2,3.424,1.112,1.0,0);
    drawGear(gear2,2.947,0.562,1.0,0);
    drawGear(gear2,3.536,0.675,1.0,0);
    drawGear(gear2,2.994,0.188,1.0,0);
    drawGear(gear2,3.593,0.226,1.0,0);

    drawGear(gear2,-0.375,2.976,1.0,0);
    drawGear(gear2,-0.451,3.572,1.0,0);
    drawGear(gear2,-0.746,2.906,1.0,0);
    drawGear(gear2,-0.895,3.487,1.0,0);
    drawGear(gear2,-1.104,2.789,1.0,0);
    drawGear(gear2,-1.325,3.347,1.0,0);
    drawGear(gear2,-1.445,2.629,1.0,0);
    drawGear(gear2,-1.734,3.155,1.0,0);
    drawGear(gear2,-1.763,2.427,1.0,0);
    drawGear(gear2,-2.116,2.912,1.0,0);
    drawGear(gear2,-2.054,2.187,1.0,0);
    drawGear(gear2,-2.464,2.624,1.0,0);
    drawGear(gear2,-2.312,1.912,1.0,0);
    drawGear(gear2,-2.774,2.293,1.0,0);
    drawGear(gear2,-2.553,1.607,1.0,0);
    drawGear(gear2,-3.040,1.929,1.0,0);
    drawGear(gear2,-2.714,1.277,1.0,0);
    drawGear(gear2,-3.257,1.533,1.0,0);
    drawGear(gear2,-2.853,0.927,1.0,0);
    drawGear(gear2,-3.424,1.112,1.0,0);
    drawGear(gear2,-2.947,0.562,1.0,0);
    drawGear(gear2,-3.536,0.675,1.0,0);
    drawGear(gear2,-2.994,0.188,1.0,0);
    drawGear(gear2,-3.593,0.226,1.0,0);

    drawGear(gear2,0.375,-2.976,1.0,0);
    drawGear(gear2,0.451,-3.572,1.0,0);
    drawGear(gear2,0.746,-2.906,1.0,0);
    drawGear(gear2,0.895,-3.487,1.0,0);
    drawGear(gear2,1.104,-2.789,1.0,0);
    drawGear(gear2,1.325,-3.347,1.0,0);
    drawGear(gear2,1.445,-2.629,1.0,0);
    drawGear(gear2,1.734,-3.155,1.0,0);
    drawGear(gear2,1.763,-2.427,1.0,0);
    drawGear(gear2,2.116,-2.912,1.0,0);
    drawGear(gear2,2.054,-2.187,1.0,0);
    drawGear(gear2,2.464,-2.624,1.0,0);
    drawGear(gear2,2.312,-1.912,1.0,0);
    drawGear(gear2,2.774,-2.293,1.0,0);
    drawGear(gear2,2.553,-1.607,1.0,0);
    drawGear(gear2,3.040,-1.929,1.0,0);
    drawGear(gear2,2.714,-1.277,1.0,0);
    drawGear(gear2,3.257,-1.533,1.0,0);
    drawGear(gear2,2.853,-0.927,1.0,0);
    drawGear(gear2,3.424,-1.112,1.0,0);
    drawGear(gear2,2.947,-0.562,1.0,0);
    drawGear(gear2,3.536,-0.675,1.0,0);
    drawGear(gear2,2.994,-0.188,1.0,0);
    drawGear(gear2,3.593,-0.226,1.0,0);


    drawGear(gear2,-0.375,-2.976,1.0,0);
    drawGear(gear2,-0.451,-3.572,1.0,0);
    drawGear(gear2,-0.746,-2.906,1.0,0);
    drawGear(gear2,-0.895,-3.487,1.0,0);
    drawGear(gear2,-1.104,-2.789,1.0,0);
    drawGear(gear2,-1.325,-3.347,1.0,0);
    drawGear(gear2,-1.445,-2.629,1.0,0);
    drawGear(gear2,-1.734,-3.155,1.0,0);
    drawGear(gear2,-1.763,-2.427,1.0,0);
    drawGear(gear2,-2.116,-2.912,1.0,0);
    drawGear(gear2,-2.054,-2.187,1.0,0);
    drawGear(gear2,-2.464,-2.624,1.0,0);
    drawGear(gear2,-2.312,-1.912,1.0,0);
    drawGear(gear2,-2.774,-2.293,1.0,0);
    drawGear(gear2,-2.553,-1.607,1.0,0);
    drawGear(gear2,-3.040,-1.929,1.0,0);
    drawGear(gear2,-2.714,-1.277,1.0,0);
    drawGear(gear2,-3.257,-1.533,1.0,0);
    drawGear(gear2,-2.853,-0.927,1.0,0);
    drawGear(gear2,-3.424,-1.112,1.0,0);
    drawGear(gear2,-2.947,-0.562,1.0,0);
    drawGear(gear2,-3.536,-0.675,1.0,0);
    drawGear(gear2,-2.994,-0.188,1.0,0);
    drawGear(gear2,-3.593,-0.226,1.0,0);

#endif
    glPopMatrix();
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport(0, (height - side) / 2,(GLint)width,(GLint)height);//设置视口的大小
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_F1)
    {
        emit sigFullScreen();
    }
    if(event->key()==Qt::Key_Escape)
    {
       emit sigQuitFullScreen();
    }

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        xRot+=8*dy;
        yRot+=8*dx;
    }
    else if (event->buttons() & Qt::RightButton)
    {
        xRot+=8*dy;
        zRot+=8*dx;
    }
    updateGL();
    lastPos = event->pos();
}

//***********************************************************************************************
bool ok;
void GLWidget::advanceGears()
{
    qbya = com.readAll();
     countQbyte=qbya.size();
     memset(readPortData,0,sizeof(readPortData));
     QDataStream out(&qbya,QIODevice::ReadWrite);
      j=0;
      while(!out.atEnd())
      {
          if(j>=countQbyte)
              break;
          out>>outChar;
          readPortData[j] = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUInt(&ok,16);
          emit sigInputDisplayLed(readPortData[j]);
          if(j==0&&flagPortData==true)
          {
              flagPortData=false;
              readLastData=readPortData[j];
              j++;
              continue;
          }

          if( ((readLastData&0x01)==0x01) && ((readPortData[j]&0x01)==0x0) )//主电机有高低电平的变化
          {
              if(upDown>=2.0)
              {
                  disconnect(&com,SIGNAL(readyRead()),this,SLOT(advanceGears()));
                  com.readAll();
                 int Yes= QMessageBox::warning(this,"警告","请抬起取样试管",QMessageBox::Yes);
                 if(Yes==QMessageBox::Yes)
                 {
                     com.readAll();
                     connect(&com,SIGNAL(readyRead()),this,SLOT(advanceGears()));
                 }
                 return;
              }
              if((readPortData[j]&0x02)==0x02)//dir位是1,向前
              {
                  gear1Rot +=4.80;
                  countDiskSpin++;
                  if( countDiskSpin>=390  &&  countDiskSpin<=410)
                  {
                      writePortData = writePortData&0xF7;
                      if(countDiskSpin==410)
                            countDiskSpin=0;
                  }
                  updateGL();
                  readLastData=readPortData[j];
                  j++;
                 continue;
              }
              else if((readPortData[j]&0x02)==0x0)//dir位是0,向后
              {
                  gear1Rot -=4.80;
                  countDiskSpin--;
                  if( countDiskSpin>=390  &&  countDiskSpin<=410)
                  {
                      writePortData = writePortData&0xF7;
                      if(countDiskSpin==410)
                            countDiskSpin=0;
                  }
                  updateGL();
                  readLastData=readPortData[j];
                  j++;
                 continue;
              }
          }

          if( ((readLastData&0x08)==0x08) && ((readPortData[j]&0x08)==0x0) )//辅电机有高低电平的变化
          {
              if((readPortData[j]&0x04)==0x04)//向下走
              {
                  if( upDown>=0.0&&upDown<=3.85)//3.85是约等于值,实际的下限值是3.8495
                  {
                      if(upDown>=3.84)
                      {
                          upDown=3.84;
                          writePortData = writePortData&0xDF;
                          goto loop;
                      }
                      upDown=upDown+0.0096;
                     qDebug()<<upDown;
                  }
                  updateGL();
              }
              else if((readPortData[j]&0x04)==0x0)//向上走
              {
                  if(upDown<=0.0)
                  {
                      upDown=0.0;
                      writePortData = writePortData&0xEF;
                      goto loop;
                  }
                  upDown=upDown-0.0096;
               }
               updateGL();
         }

loop: if(writePortData!=0xFF)
         {
             emit sigOutputDisplayLed(writePortData);
             strToHex=QChar(writePortData);
             com.write(strToHex.toLatin1());
         }
          readLastData=readPortData[j];
          writePortData=0xFF;
      }//while
}

GLuint GLWidget::makeGear(const GLfloat *reflectance, GLdouble innerRadius,
                          GLdouble outerRadius, GLdouble thickness,
                          GLdouble toothSize, GLint toothCount)
{
    const double Pi = 3.14159265358979323846;

    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectance);

    GLdouble r0 = innerRadius;
    GLdouble r1 = outerRadius - toothSize / 2.0;
    GLdouble r2 = outerRadius + toothSize / 2.0;
    GLdouble delta = (2.0 * Pi / toothCount) / 4.0;
    GLdouble z = thickness / 2.0;
    int i, j;

    glShadeModel(GL_FLAT);

    for (i = 0; i < 2; ++i) {
        GLdouble sign = (i == 0) ? +1.0 : -1.0;

        glNormal3d(0.0, 0.0, sign);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= toothCount; ++j) {
            GLdouble angle = 2.0 * Pi * j / toothCount;
	    glVertex3d(r0 * cos(angle), r0 * sin(angle), sign * z);
	    glVertex3d(r1 * cos(angle), r1 * sin(angle), sign * z);
	    glVertex3d(r0 * cos(angle), r0 * sin(angle), sign * z);
	    glVertex3d(r1 * cos(angle + 3 * delta), r1 * sin(angle + 3 * delta),
                       sign * z);
        }
        glEnd();

        glBegin(GL_QUADS);
        for (j = 0; j < toothCount; ++j) {
            GLdouble angle = 2.0 * Pi * j / toothCount;
	    glVertex3d(r1 * cos(angle), r1 * sin(angle), sign * z);
	    glVertex3d(r2 * cos(angle + delta), r2 * sin(angle + delta),
                       sign * z);
	    glVertex3d(r2 * cos(angle + 2 * delta), r2 * sin(angle + 2 * delta),
                       sign * z);
	    glVertex3d(r1 * cos(angle + 3 * delta), r1 * sin(angle + 3 * delta),
                       sign * z);
        }
        glEnd();
    }

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < toothCount; ++i) {
        for (j = 0; j < 2; ++j) {
            GLdouble angle = 2.0 * Pi * (i + (j / 2.0)) / toothCount;
            GLdouble s1 = r1;
            GLdouble s2 = r2;
            if (j == 1)
                qSwap(s1, s2);

	    glNormal3d(cos(angle), sin(angle), 0.0);
	    glVertex3d(s1 * cos(angle), s1 * sin(angle), +z);
	    glVertex3d(s1 * cos(angle), s1 * sin(angle), -z);

	    glNormal3d(s2 * sin(angle + delta) - s1 * sin(angle),
                       s1 * cos(angle) - s2 * cos(angle + delta), 0.0);
	    glVertex3d(s2 * cos(angle + delta), s2 * sin(angle + delta), +z);
	    glVertex3d(s2 * cos(angle + delta), s2 * sin(angle + delta), -z);
        }
    }
    glVertex3d(r1, 0.0, +z);
    glVertex3d(r1, 0.0, -z);
    glEnd();

    glShadeModel(GL_SMOOTH);

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= toothCount; ++i)
    {
        GLdouble angle = i * 2.0 * Pi / toothCount;
        glNormal3d(-cos(angle), -sin(angle), 0.0);
        glVertex3d(r0 * cos(angle), r0 * sin(angle), +z);
        glVertex3d(r0 * cos(angle), r0 * sin(angle), -z);
    }
    glEnd();
    glEndList();
    return list;
}

void GLWidget::drawGear(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz,
                        GLdouble angle)
{
    glPushMatrix();//保存当前坐标
    glTranslated(dx, dy, dz);//移动坐标到指定位置
    glCallList(gear);
    glPopMatrix();//恢复保存的坐标
    glRotated(angle, 0.0, 0.0, 1.0);
}

void GLWidget::drawGear2(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz)
{
    glPushMatrix();//保存当前坐标
    glTranslated(dx, dy, dz);//移动坐标到指定位置
    glCallList(gear);
    glPopMatrix();//恢复保存的坐标
}

//-------------------------------------------------------------------------------------------
void GLWidget::drawGear3(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz)
{
    glPushMatrix();//保存当前坐标
    if(upDown>=0.0&&upDown<=4.0)
    {
        glTranslated(dx, dy+upDown, dz);//移动坐标到指定位置

    }
    else if(upDown>=-4.0&&upDown<0.0)
    {
         glTranslated(dx, dy-upDown, dz);

    }
    glCallList(gear);
    glPopMatrix();//恢复保存的坐标
}
//---------------------------------------------------------------------------------------------

void GLWidget::drawGear4(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz)
{
    glPushMatrix();//保存当前坐标

    if(upDown>=0.0&&upDown<=4.0)
    {
        glTranslated(dx, dy, dz-upDown);//移动坐标到指定位置
    }
    else if(upDown>=-4.0&&upDown<0.0)
    {
         glTranslated(dx, dy, dz+upDown);
    }

    glCallList(gear);

    glPopMatrix();//恢复保存的坐标

}

void GLWidget::drawGear5(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz)
{

    glPushMatrix();//保存当前坐标

    if(upDown>=0.0&&upDown<=4.0)
    {
        glTranslated(dx, dy, dz-upDown);//移动坐标到指定位置
    }
    else if(upDown>=-4.0&&upDown<0.0)
    {
         glTranslated(dx, dy, dz+upDown);
    }
    glCallList(gear);
    glPopMatrix();//恢复保存的坐标

}

void spinDisplay(void)
{
    if (spin >= 30)
        status = -1;
    else if(spin<=-35)
        status = 1;
   spin += status*0.05;//这一行代码用来控制移动的速度
}

void GLWidget::drawGear6(GLuint gear,GLdouble dx,GLdouble dy, GLdouble dz)
{
    glPushMatrix();
    glTranslated(dx,dy,dz);
    glCallList(gear);
    glPopMatrix();
}

void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}
