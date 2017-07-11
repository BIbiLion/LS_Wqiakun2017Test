#include "basicusagescene.h"

#include "glassert.h"

#include <QObject>
#include <QOpenGLContext>


BasicUsageScene::BasicUsageScene()
    : mShaderProgram(),
      mVertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      mVertexColorBuffer(QOpenGLBuffer::VertexBuffer)
{
    m_time= new QTime();
    m_time->start();
}

void BasicUsageScene::initialize()
{
    prepareShaderProgram();
    prepareVertexBuffers();

    //纹理
    prepareTex();
}


void BasicUsageScene::update(float t)
{
    Q_UNUSED(t);
}

#include <QMatrix>
#include <QMatrix4x4>
void BasicUsageScene::render()
{
    texture->bind();

    glAssert( glClear(GL_COLOR_BUFFER_BIT) );

    glViewport(0, 0, 300, 300);
    QMatrix4x4 _mat;
    _mat.rotate((float) m_time->currentTime().second(),QVector3D(0.0, 0.0, 0.1));

    mShaderProgram.bind();
    int _location=  mShaderProgram.uniformLocation("transform");
    mShaderProgram.setUniformValue(_location,_mat);
    mVAO.bind();

    glAssert( glDrawArrays(GL_TRIANGLES, 0, 4) );
    mVAO.release();



    //绘制不同的视口
    glViewport(300, 300, 600, 600);
    QMatrix4x4 _mat2;
    _mat2.rotate((float)( m_time->currentTime().msec()/3),QVector3D(0.0, 0.0, 0.1));

    mShaderProgram.bind();
    _location=  mShaderProgram.uniformLocation("transform");
    mShaderProgram.setUniformValue(_location,_mat2);
    mShaderProgram.bind();
    mVAO.bind();

    glAssert( glDrawArrays(GL_TRIANGLE_STRIP, 0, 4) );
    mVAO.release();
}

void BasicUsageScene::resize(int width, int height)
{

    glAssert( glViewport(0, 0, width, height) );
}

void BasicUsageScene::prepareShaderProgram()
{
    if (!mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/phong.vert"))
    {
        qCritical() << "error";
    }
    if (!mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/phong.frag"))
    {
        qCritical() << "error";
    }
    if (!mShaderProgram.link())
    {
        qCritical() << "error";
    }
    glCheckError();
}

#include <QImage>
void BasicUsageScene::prepareTex()
{

    glEnable( GL_TEXTURE_2D);
    texture = new QOpenGLTexture(QImage("./backgroud-image.png").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    mShaderProgram.setUniformValue("ourTexture", 0);
    texture->bind();
}

void BasicUsageScene::prepareVertexBuffers()
{
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f,  10.0f, 0.0f,
        100.0f, 100.0f,100.0f
    };
    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f,0.7f,0.3f
    };

    //add
    float texCodData[]=
    { 0.0f,0.0f,
      0.0f,1.0f,
      1.0f,0.0f,
      1.0f,1.0f
    };


    mVAO.create();
    mVAO.bind();


    mVertexPositionBuffer.create();
    mVertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mVertexPositionBuffer.bind();
    mVertexPositionBuffer.allocate(positionData, 4 * 3 * sizeof(float));

    mVertexColorBuffer.create();
    mVertexColorBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mVertexColorBuffer.bind();
    mVertexColorBuffer.allocate(colorData, 4 * 3 * sizeof(float));


    //add
    mVertexTexBuffer.create();
    mVertexTexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mVertexTexBuffer.bind();
    mVertexTexBuffer.allocate(texCodData, 4* 2 * sizeof(float));



    mShaderProgram.bind();

    mVertexPositionBuffer.bind();
    mShaderProgram.enableAttributeArray("vertexPosition");
    mShaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

    mVertexColorBuffer.bind();
    mShaderProgram.enableAttributeArray("vertexColor");
    mShaderProgram.setAttributeBuffer("vertexColor", GL_FLOAT, 0, 3);

    //add Textture
    mVertexTexBuffer.bind();
    mShaderProgram.enableAttributeArray("aTexCoord");
    mShaderProgram.setAttributeBuffer("aTexCoord", GL_FLOAT, 0, 2);



    mVAO.release();
    qDebug()<<mShaderProgram.log();
    glCheckError();
}
