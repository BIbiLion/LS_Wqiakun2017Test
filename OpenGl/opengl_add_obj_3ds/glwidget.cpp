#include "glwidget.h"
#include "ui_glwidget.h"
#include "gl/GLU.h"
#include "MeshModel.h"

cMeshModel *m_chanche, *m_sphere;
extern int mWindowWidth;
extern int mWindowHeight;

GLWidget::GLWidget(QGLWidget *parent) :
    QGLWidget(parent),
    ui(new Ui::GLWidget)
{
    ui->setupUi(this);

    setWindowTitle("Load Model");
    setGeometry(100,100,800,600);
}

GLWidget::~GLWidget()
{
    delete ui;
}

void GLWidget::initializeGL()
{
    glClearColor(1.0f,1.0f,0.0,1.0f);

    //matrix.
    matrix.perspective(45.0f,(GLfloat)mWindowWidth/mWindowHeight,0.1f,100.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Load Model
    m_sphere = new cMeshModel(QString("sphere.obj"));
    m_chanche = new cMeshModel(QString("chanche.3ds"));
}

void GLWidget::resizeGL(int w, int h)
{
    mWindowWidth = w;
    mWindowHeight = (h==0)?1:h;

    glViewport(0,0,mWindowWidth,mWindowHeight);
    paintGL();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    QMatrix4x4 ccMatrix = matrix;
    ccMatrix.translate(QVector3D(0,0,-2.4f));
    ccMatrix.rotate(90.0f,QVector3D(0,1,0));

    // Render
    m_chanche->Render(ccMatrix);

    ccMatrix = matrix;
    ccMatrix.translate(QVector3D(0.8f,0.6f,-2.4f));
    ccMatrix.scale(QVector3D(0.5f,0.5f,0.5f));
    m_sphere->Render(ccMatrix);
}
