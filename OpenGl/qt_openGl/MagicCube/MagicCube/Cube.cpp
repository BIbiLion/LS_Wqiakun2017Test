#include "Cube.h"

//构造函数,初始化Cube类(动画)的各种属相
Cube::Cube(QWidget *parent) : QGLWidget(parent)
{
    x=y=z=0.0;
    xTimer=yTimer=zTimer=0;
    yFlagLeft=true;
    yFlagRight=false;
    xFlagBefor=false;
    xFlagAfter=false;
    translate=-5.0;

   connect(&GLTimer,SIGNAL(timeout()),this,SLOT(changePos()));
   GLTimer.start(100);
}

//贴出
void Cube::initTextures()
{
    textures[0]=bindTexture(QPixmap("../MagicCube/miyan.jpg"));
    textures[1]=bindTexture(QPixmap("../MagicCube/xiaonan.jpg"));
    textures[2]=bindTexture(QPixmap("../MagicCube/didala.jpg"));
    textures[3]=bindTexture(QPixmap("../MagicCube/xie.jpg"));
    textures[4]=bindTexture(QPixmap("../MagicCube/guijiao.jpg"));
    textures[5]=bindTexture(QPixmap("../MagicCube/yuzhiboyou.jpg"));
    glEnable(GL_TEXTURE_2D);//启用2D贴图功能
}

//灯光效果,让动画看起来更逼真
void Cube::initLight()
{
    /*创建光源的过程和颜色的创建完全一致。前三个参数分别是RGB三色分量，最后一个是alpha通道参数*/
    GLfloat lightAmbient[4]={0.5, 0.5, 0.5,1.0};//这是"环境光",环境光来自于四面八方
    GLfloat lightDisffuse[4]={1.0, 1.0, 1.0, 1.0};//这是"漫射光",漫射光由特定的光源产生，并在您的场景中的对象表面上产生反射。处于漫射光直接照射下的任何对象表面都变得很亮，而几乎未被照射到的区域就显得要暗一些。
    GLfloat lightPosition[4]={0.0, 0.0, 1.0, 1.0};//关lightPosition的代码使我们保存光源的位置。前三个参数和glTranslate中的一样。依次分别是XYZ轴上的位移,最后一个参数取为1.0。这将告诉OpenGL这里指定的坐标就是光源的位置

    glLightfv(GL_LIGHT1,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDisffuse);
    glLightfv(GL_LIGHT1,GL_POSITION,lightPosition);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

//初始化3D场景,固定代码
void Cube::initializeGL()
{
    glClearColor(0.0,0.0,0.0,0.0);
    //glClearColor(0.5,0.5,0.5,1.0);//要启用雾效果就用这句来初始化屏幕颜色
    glShadeModel(GL_SMOOTH);//设置阴影平滑
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    initTextures();
}

//决定3D动画的屏幕尺寸
void Cube::resizeGL(int w, int h)
{
    glViewport(0,0,(GLint)w,(GLint)h);//设置视口的大小
    glMatrixMode(GL_PROJECTION);//选择投影矩阵,接下来的两行代码将影响"投影矩阵"
    glLoadIdentity();
    gluPerspective(45.0,(GLfloat)w/(GLfloat)w,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);//设置了投影矩阵,增加了透视效果
    glLoadIdentity();//可以将投影矩阵恢复到初始状态
}

//在3D场景里画图
void Cube::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, translate);
    glRotatef(x,1,0,0);
    glRotatef(y,0,1,0);
    glRotatef(z,0,0,1);

    //顶面
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    glBegin(GL_QUADS);
    //glColor3f(0.0, 1.0, 0.0);
    glTexCoord2f(1.0,1.0); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2f(0.0,0.0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(1.0, 1.0, 1.0);
    //glColor3f(1.0, 1.0, 1.0);
    glEnd();

    //底面
    glBindTexture(GL_TEXTURE_2D,textures[1]);
    glBegin(GL_QUADS);
    //glColor3f(1.0, 0.5, 0.0);
    glTexCoord2f(1.0,1.0); glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(0.0,0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(1.0, -1.0, -1.0);
    //glColor3f(1.0, 1.0, 1.0);
    glEnd();

    //前面
    glBindTexture(GL_TEXTURE_2D,textures[2]);
    glBegin(GL_QUADS);
    //glColor3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0,1.0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(0.0,0.0); glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(1.0, -1.0, 1.0);
    glEnd();

    //后面
    glBindTexture(GL_TEXTURE_2D,textures[3]);
    glBegin(GL_QUADS);
    //glColor3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0,1.0); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2f(0.0,0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(1.0, -1.0, -1.0);
    glEnd();

    //左面
    glBindTexture(GL_TEXTURE_2D,textures[4]);
    glBegin(GL_QUADS);
    //glColor3f(0.0, 0.0, 1.0);
    glTexCoord2f(1.0,1.0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2f(0.0,0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(-1.0, -1.0, 1.0);
    glEnd();

    //右面
    glBindTexture(GL_TEXTURE_2D,textures[5]);
    glBegin(GL_QUADS);
    //glColor3f(1.0, 0.0, 1.0);
    glTexCoord2f(1.0,1.0); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(0.0,1.0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0,0.0); glVertex3f(1.0, -1.0,1.0);
    glTexCoord2f(1.0,0.0); glVertex3f(1.0, -1.0, -1.0);
    glEnd();
}

void Cube::changePos()
{
    if(yFlagLeft ==true)
    {
        y-=3;
    }
    else if(yFlagRight==true)
    {
        y+=3;
    }
    else if(xFlagBefor==true)
    {
        x+=3;
    }
    else if(xFlagAfter==true)
    {
        x-=3;
    }
    updateGL();
}

void Cube::Action(QString strAct)
{
    QLabel *lab = new QLabel;
    lab->setText(strAct);
    lab->show();
    //qDebug()<<"strAct:"<<strAct<<endl;
    if(strAct.contains("左")||strAct.contains("佐"))
    {
        yFlagLeft=true;
        yFlagRight=false;
        xFlagBefor=false;
        xFlagAfter=false;
    }

    else if(strAct.contains("右")||strAct.contains("佑")||strAct.contains("右"))
    {
         yFlagRight=true;
         yFlagLeft=false;
         xFlagBefor=false;
         xFlagAfter=false;
         //qDebug()<<"gaiguolaile"<<endl;
    }

    else if(strAct.contains("前")||strAct.contains("钱")||strAct.contains("千"))
    {
        xFlagBefor=true;
        yFlagLeft=false;
        yFlagRight=false;
        xFlagAfter=false;
    }

    else if(strAct.contains("后"))
    {
        xFlagAfter=true;
        yFlagLeft=false;
        yFlagRight=false;
        xFlagBefor=false;
    }

}

void Cube::changeX(int valueX)
{
    x=valueX;
    updateGL();
}

void Cube::changeY(int valueY)
{
    y=valueY;
    updateGL();
}

void Cube::changeZ(int valueZ)
{
    z=valueZ;
    updateGL();
}

Cube::~Cube()
{

}

