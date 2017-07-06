#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    this->setAutoFillBackground(false);
    initVariable();
    //lab.setParent(this);
    lab.setText("串口不可用");
    //connect(&com,SIGNAL(readyRead()),this,SLOT(findCom()));
     int comNum=0;
   /*while(comNum<=4)
    {

        strComName="COM"+QString::number(comNum);
        findCom();
        comNum++;
    }*/
    strComName="COM2";
    findCom();
}

void GLWidget::initializeGL()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_SMOOTH);//设置阴影平滑
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);//使用第0号光照.当然有1,2,3.....号光照,看你opengl的光照支持多少了!
    glEnable(GL_LIGHTING);//启用光照
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,(GLint)w,(GLint)h);//设置视口的大小
    glMatrixMode(GL_PROJECTION);//选择投影矩阵,接下来的两行代码将影响"投影矩阵"
    glLoadIdentity();
    gluPerspective(45.0,(GLfloat)w/(GLfloat)w,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);//设置了投影矩阵,增加了透视效果
    glLoadIdentity();//可以将投影矩阵恢复到初始状态
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0, 0.0, translate);
    glColor3f(0.0, 0.0, 1.0);
    glRotatef(x/16,1,0,0);
    glRotatef(y/16,0,1,0);
    //glRotatef(-90,0,1,0);

    quar= gluNewQuadric();
    //画大圆柱
    GLdouble big_cylinder_size[5]={2.0,2.0,3.0,32,32};
    GLfloat big_cylinder_body_color[3]={0.0, 0.0, 1.0};
    GLfloat big_cylinder_color[3]={0.0, 1.0, 1.0};
    GLfloat big_cylinder_position[3]={0.0, 0.0, 3.0};
    paintCylinder(big_cylinder_size,big_cylinder_body_color , big_cylinder_color, big_cylinder_position);

    //画中圆柱
    GLdouble mid_cylinder_size[5]={1.0,1.0,1.5,32,32};
    GLfloat mid_cylinder_body_color[3]={0.5, 0.5, 0.5};
    GLfloat mid_cylinder_color[3]={0.8, 0.8, 0.8};
    GLfloat mid_cylinder_position[3]={0.0, 0.0, 1.5};
    paintCylinder(mid_cylinder_size, mid_cylinder_body_color, mid_cylinder_color, mid_cylinder_position);

    //画小圆柱
    GLdouble small_cylinder_size[5]={0.3,0.3,0.7,32,32};
    GLfloat small_cylinder_body_color[3]={0.0, 0.0, 1.0};
    GLfloat small_cylinder_color[3]={0.0, 0.5, 1.0};
    GLfloat small_cylinder_position[3]={0.0, 0.0, 0.7};
    paintCylinder(small_cylinder_size, small_cylinder_body_color, small_cylinder_color, small_cylinder_position);

    /****************************************齿轮*************************************************************/
    //glLoadIdentity();
    //glTranslatef(0.0, 0.0,0.0);
       glColor3f(0.0,0.0,1.0);
       glScalef(0.4, 0.4, 0.4);
       glRotatef(z,0,0,1);
    //glCallList(gear1);
       glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,0);
       //GLfloat lightPos0[4] = { 7.0f, 7.0f, 3.0f, 1.0f };
      //glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);//这句是指定第0号光源的位置

    //glEnable(GL_CULL_FACE);//这句的意思是:无论角度怎么变换,都优先显示该"该语句所在的图形",即保证无论角度怎么转,都能看到该图形,不会被其他图形所遮挡*/
    //gear1 = glGenLists(1);
    //glNewList(gear1, GL_COMPILE);
   //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectance1);

       gear(1.0, 4.0, 1.0, 50, 0.7);
    //glEndList();
    //glCallList(gear1);
       glColor3f(1.0,1.0,1.0);
}

void GLWidget::gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,GLint teeth, GLfloat tooth_depth)
{
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth / 2.0;
    r2 = outer_radius + tooth_depth / 2.0;

    da = 2.0 * M_PI / teeth / 4.0;

    glShadeModel(GL_FLAT);
    glNormal3f(0.0, 0.0, 1.0);


    glBegin(GL_QUAD_STRIP);
        for(i = 0; i <= teeth; i++)
        {    angle = i * 2.0 * M_PI / teeth;
             glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
             glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);

            if(i < teeth)
            {    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
                glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
            }
        }
    glEnd();


    glBegin(GL_QUADS);
        da = 2.0 * M_PI / teeth / 4.0;
        for(i = 0; i < teeth; i++)
        {    angle = i * 2.0 * M_PI / teeth;

            glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
            glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
            glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
            glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
        }
    glEnd();

    glNormal3f(0.0, 0.0, -1.0);


    glBegin(GL_QUAD_STRIP);
        for(i = 0; i <= teeth; i++)
        {    angle = i * 2.0 * M_PI / teeth;
            glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
            glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
            if(i < teeth)
            {    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
                glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
            }
        }
    glEnd();


    glBegin(GL_QUADS);
        da = 2.0 * M_PI / teeth / 4.0;
        for(i = 0; i < teeth; i++)
        {    angle = i * 2.0 * M_PI / teeth;

            glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
            glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
            glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
            glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
        }
    glEnd();


    glBegin(GL_QUAD_STRIP);
        for(i = 0; i < teeth; i++)
        {    angle = i * 2.0 * M_PI / teeth;

            glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
            glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
            u = r2 * cos(angle + da) - r1 * cos(angle);
            v = r2 * sin(angle + da) - r1 * sin(angle);
            len = sqrt(u * u + v * v);
            u /= len;
            v /= len;
            glNormal3f(v, -u, 0.0);
            glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
            glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
            glNormal3f(cos(angle), sin(angle), 0.0);
            glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
            glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
            u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
            v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
            glNormal3f(v, -u, 0.0);
            glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
            glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
            glNormal3f(cos(angle), sin(angle), 0.0);
        }
        glVertex3f(r1 * cos(0.0f), r1 * sin(0.0f), width * 0.5);
        glVertex3f(r1 * cos(0.0f), r1 * sin(0.0f), -width * 0.5);
    glEnd();

    glShadeModel(GL_SMOOTH);


    glBegin(GL_QUAD_STRIP);
        for(i = 0; i <= teeth; i++)
        {    angle = i * 2.0 * M_PI / teeth;
            glNormal3f(-cos(angle), -sin(angle), 0.0);
            glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
            glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
        }
    glEnd();
}

void GLWidget::paintCylinder(GLdouble size[],GLfloat body_color[],GLfloat color[],GLfloat position[])
{
    glBegin(GL_QUADS);
    glColor3f(body_color[0],body_color[1],body_color[2]);
    gluQuadricDrawStyle(quar,GLU_FILL);//最后一个参数是画实体面,还有线面,点面
    gluCylinder(quar,size[0],size[1],size[2],size[3],size[4]);

    glColor3f(color[0],color[1],color[2]);
    gluDisk(quar,0.0,size[0],size[3],size[4]);

    glTranslatef(position[0],position[1],position[2]);
    glColor3f(color[0],color[1],color[2]);
    gluDisk(quar,0.0,size[0],size[3],size[4]);
    glEnd();
}

void GLWidget::keyPressEvent(QKeyEvent* e)
{
    if(e->key()==Qt::Key_Up)
        x+=10;
    if(e->key()==Qt::Key_Left)
    {
        z-=0.9;
       countGearSpin--;
       if(countGearSpin==-400 || countGearSpin==0)
       {
           countGearSpin=0;
           strToHex=QChar(0xEF);
           com.write(strToHex.toLatin1());
           qDebug()<<"yiquan le "<<endl;
       }
    }
    if(e->key()==Qt::Key_Right)
    {
        z+=0.9;
        countGearSpin++;
        if(countGearSpin==400 || countGearSpin==0)
        {
            countGearSpin=0;
            strToHex=QChar(0xEF);
            com.write(strToHex.toLatin1());
        }
    }
    if(e->key()==Qt::Key_Down)
    {
        translate-=1;
        if(translate<=-50)
            translate=-1;
    }
    if(e->key()==Qt::Key_F1)
    {
        emit sigFullScreen();
    }
    if(e->key()==Qt::Key_Escape)
    {
       emit sigQuitFullScreen();
    }
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->x() - lastPos.x();
    int dy = e->y() - lastPos.y();

    if(e->buttons() & Qt::LeftButton)
    {
       x+=8*dy;
       y+=8*dx;
    }
    if(e->buttons() & Qt::RightButton)
    {
        x+=8*dy;
        z+=8*dx;
    }
   updateGL();
    lastPos=e->pos();
}

void GLWidget::initVariable()
{
    /*************** openGL用到的 *****************************/
    x=y=z= 0.0;
   translate=-15;

    /*************** 串口处理数据用到的 ***********************/
    j=0;
    countQbyte=0;
    countGearSpin=0;
    //comNum=0;
    lastData=0;
    writePortData=0xFF;
    flagPortData=true;
    memset(portData,0,sizeof(portData));
}

bool ok;
void GLWidget::startGearSpin()
{
    writePortData=0xFF;
    qbya = com.read(20);
    countQbyte=qbya.size();
    if(countQbyte<=0)
        return;
    memset(portData,0,sizeof(portData));
    QDataStream out(&qbya,QIODevice::ReadWrite);
     j=0;
    while(!out.atEnd())
    {
        if(j>=countQbyte)
            break;
        out>>outChar;
        portData[j] = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUInt(&ok,16);//qDebug()<<j<<portData[j]<<ok;
        emit sigDisplayLed6to0(portData[j]);
        if(j==0&&flagPortData==true)
        {
            flagPortData=false;
            lastData=portData[j];
            j++;
            continue;
        }
        else if( ((lastData&0x01)==1) && ((portData[j]&0x01)==0) )
        {
            emit sigDisplayLed7((uint)0x80);
           //qDebug()<<"检测到高低电平的变化"; //qDebug()<<"portData[i]: "<<i<<portData[i]<<"portData[i+1]: "<<portData[i+1];
            if((portData[j]&0x02)==0x02)//第1位:1
            {
                //qDebug()<<"向前转"<<endl;
                z+=0.9;
                countGearSpin++;
                if(countGearSpin==400 || countGearSpin==0)
                {
                    countGearSpin=0;
                    strToHex=QChar(0x7F);
                    com.write(strToHex.toLatin1());
                    qDebug()<<QTime::currentTime().toString()<<endl;
                    emit sigDisplayLed7((uint)0x7F);
                }
                updateGL();
                lastData=portData[j];
                j++;
                continue;
            }
            if((portData[j]&0x02)==0x00)//第1位:0
            {
                //qDebug()<<"向后转"<<endl;
                z-=0.9;
                countGearSpin--;
                if(countGearSpin==-400 || countGearSpin==0)
                {
                    countGearSpin=0;
                    strToHex=QChar(0x7F);
                    com.write(strToHex.toLatin1());
                    qDebug()<<QTime::currentTime().toString()<<endl;
                    emit sigDisplayLed7((uint)0x7F);
                }
                lastData=portData[j];
                updateGL();
                j++;
                continue;
            }
        }//else if
        lastData=portData[j];
        //qDebug()<<"lastData: "<<lastData<<endl;
    }
}

void GLWidget::findCom()
{
    if(com.bytesAvailable()>0)
    {        
        if(com.readAll().toHex()=="ff")
        {
            disconnect(&com,SIGNAL(readyRead()),this,SLOT(findCom()));
            //connect(&com,SIGNAL(readyRead()),this,SLOT(startGearSpin()));//使用信号和槽
            lab.setText("找到"+strComName+"可用");
        }
    }
    connect(&com,SIGNAL(readyRead()),this,SLOT(startGearSpin()),Qt::QueuedConnection);//使用信号和槽
    com.close();
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
}

GLWidget::~GLWidget()
{

}

