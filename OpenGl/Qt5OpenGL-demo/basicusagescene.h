#ifndef BASICUSAGESCENE_H
#define BASICUSAGESCENE_H

#include "abstractscene.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <QImage>
#include <QTime>
#include <QOpenGLTexture>
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

    void prepareShaderProgram();
    void prepareVertexBuffers();
    void prepareTex();


    QOpenGLTexture * texture;
      unsigned int texture1;
       QImage _tex;

       QTime * m_time;
};

#endif // BASICUSAGESCENE_H
