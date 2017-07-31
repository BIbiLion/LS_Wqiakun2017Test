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
#include <QOpenGLFunctions>

struct TorusData
{
    TorusData() :
        tubeRadius(0.3), torusRadius(1.0),
        tubeResolution(20), torusResolution(40),
        vertexBuffer(0), indexBuffer(0), shader(0),
        angle(0) { }
    ~TorusData() {
        delete vertexBuffer;
        delete indexBuffer;
        delete shader;
    }

    const float tubeRadius;
    const float torusRadius;
    const int tubeResolution;
    const int torusResolution;

    QOpenGLTexture *environment;
    QOpenGLBuffer *vertexBuffer;
    QOpenGLBuffer *indexBuffer;
    QOpenGLShaderProgram *shader;

    float angle;
};
//[Glwidget]
//class BasicUsageScene : public /*AbstractScene ,protected*/ QOpenGLFunctions
class BasicUsageScene : public AbstractScene ,protected QOpenGLFunctions
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
    void prepareMaterial();

    QOpenGLTexture * texture;
    unsigned int texture1;
    QImage _tex;

    QTime * m_time;

    //cube
    void prepareCubeVerTex();

    QOpenGLVertexArrayObject mCubVAO;
    QOpenGLBuffer m_cubVertexPositionBuffer;
    QOpenGLShaderProgram mCubeShaderProgram;

    void cubeMap_skyBox();
    QOpenGLTexture * _cubMap;
    QOpenGLVertexArrayObject m_skyBox_VAO;
    QOpenGLBuffer *m_skyBox_VertexPositionBuffer;
    QOpenGLShaderProgram *m_skyBox_ShaderProgram;
    QImage *_bottomImage;
    QImage *_topImage;
    QImage *_leftImage;
    QImage *_rightImage;
    QImage *_frontImage;
    QImage *_backImage;
public:
    QMatrix4x4  m_viewMatrix;

   void renderTorus( QMatrix4x4 &viewMatrix,  QMatrix4x4 &projectionMatrix);
    void renderTorus();
    void render( QMatrix4x4 viewMatrix,  QMatrix4x4 projectionMatrix);
protected:
    void preparetorus();

  TorusData *d;

};

#endif // BASICUSAGESCENE_H
