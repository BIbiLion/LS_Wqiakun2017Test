#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "skybox.h"
#include "torus.h"
struct GLWidgetData
{
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;

    SkyBox skyBox;
    Torus torus;
};
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    //QMatrix4x4 m_projection;
    GLWidgetData *d;
};
#endif // GLWIDGET_H
