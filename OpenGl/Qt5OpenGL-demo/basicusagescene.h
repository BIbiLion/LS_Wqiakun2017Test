#ifndef BASICUSAGESCENE_H
#define BASICUSAGESCENE_H

#include "abstractscene.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <QImage>
#include <QTime>
#include <QOpenGLTexture>

#include <QMatrix4x4>
class BasicUsageScene : public AbstractScene
{
public:
    BasicUsageScene();

    virtual void initialize();
    virtual void update(float t);
    virtual void render();
    virtual void resize(int width, int height);

private:
    QOpenGLShaderProgram mShaderProgram;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVertexPositionBuffer;
    QOpenGLBuffer mVertexColorBuffer;

    QOpenGLBuffer mVertexTexBuffer;

    QOpenGLBuffer mVertexNormalBuffer;

    void prepareShaderProgram();
    void prepareVertexBuffers();
    void prepareTex();
    void prepareLight();

    QOpenGLTexture * texture;
    unsigned int texture1;
    QImage _tex;

    QTime * m_time;
public:
    QMatrix4x4  m_viewMatrix;
public slots:

};

#endif // BASICUSAGESCENE_H
