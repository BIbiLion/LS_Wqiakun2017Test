#include "glwidget.h"
#include <QMatrix4x4>



GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    d = new GLWidgetData;

    QSurfaceFormat format;
    format.setSamples(4);
    setFormat(format);
}

GLWidget::~GLWidget()
{
    delete d;
}

void GLWidget::initializeGL()
{
    QOpenGLFunctions::initializeOpenGLFunctions();

    glClearColor(0.25, 0.35, 0.45, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-0.03125f, -0.03125f);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
 qDebug()<<"比："<<w<<h;
    d->projectionMatrix.setToIdentity();
    d->projectionMatrix.perspective(60.0, float(w)/float(h), 0.1f, 20.0f);

    d->viewMatrix.setToIdentity();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

//    d->skyBox.render(d->viewMatrix, d->projectionMatrix);
//    d->torus.render(d->viewMatrix, d->projectionMatrix);
     d->scene.render();
    //d->scene.render(d->viewMatrix, d->projectionMatrix);
}
