#ifndef CUBE_H
#define CUBE_H

#include <QGLWidget>
#include <gl/glu.h>
#include <gl/glaux.h>
#include <QDebug>
#include <QTimer>
#include <QLabel>
class Cube : public QGLWidget
{
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w,int h);
    void paintGL();
    void initTextures();
    void initLight();
    ~Cube();
private:
    QTimer GLTimer;
    int xTimer,yTimer,zTimer;
    bool yFlagLeft,yFlagRight;
    bool xFlagBefor,xFlagAfter;


    volatile GLfloat x,y,z,translate;
    GLuint textures[6];

public slots:
     void changePos();
     void Action(QString strAct);
     void changeX(int valueX);
     void changeY(int valueY);
     void changeZ(int valueZ);


};

#endif // CUBE_H
