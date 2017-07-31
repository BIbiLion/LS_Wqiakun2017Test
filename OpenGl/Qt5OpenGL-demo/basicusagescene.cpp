#include "basicusagescene.h"

#include "glassert.h"

#include <QObject>
#include <QOpenGLContext>


BasicUsageScene::BasicUsageScene()
    : mShaderProgram(),
      mVertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      mVertexColorBuffer(QOpenGLBuffer::VertexBuffer),
      mCubeShaderProgram(), m_cubVertexPositionBuffer(QOpenGLBuffer::VertexBuffer)
    //, m_skyBox_ShaderProgram(),m_skyBox_VertexPositionBuffer(QOpenGLBuffer::VertexBuffer)
{
    m_time= new QTime();
    m_time->start();

    d = new TorusData;
}

#include <QOpenGLFunctions>
void BasicUsageScene::initialize()
{
    QOpenGLFunctions::initializeOpenGLFunctions();

    prepareShaderProgram();
    prepareVertexBuffers();
    //纹理
    prepareTex();
    prepareLight();
    prepareMaterial();

    //draw cubVertex
      prepareCubeVerTex();


    //cubeMap_skyBox();
    preparetorus();
}


void BasicUsageScene::update(float t)
{
    Q_UNUSED(t);
}

#include <QMatrix>
#include <QMatrix4x4>
#include <QDebug>

void BasicUsageScene::render( QMatrix4x4 viewMatrix,  QMatrix4x4 projectionMatrix)
{
    QOpenGLFunctions::initializeOpenGLFunctions();
    qDebug()<<"render";
    glClearColor(0.25, 0.35, 0.45, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    renderTorus(viewMatrix,projectionMatrix);
}
#include <QTime>
void BasicUsageScene::renderTorus( QMatrix4x4 &viewMatrix,  QMatrix4x4 &projectionMatrix)
{

      glViewport(400, 400, 800, 800);
    //preparetorus();
    d->shader->bind();
    d->vertexBuffer->bind();
    d->indexBuffer->bind();

    d->shader->enableAttributeArray("qt_Vertex");
    d->shader->setAttributeBuffer("qt_Vertex", GL_FLOAT, 0, 3, 2*sizeof(QVector3D));

    d->shader->enableAttributeArray("qt_Normal");
    d->shader->setAttributeBuffer("qt_Normal", GL_FLOAT, sizeof(QVector3D), 3, 2*sizeof(QVector3D));

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0, 0, -3.0);
    modelMatrix.rotate(d->angle, 0, 1, 0);
    d->angle += 5;

            QMatrix4x4  viewMatrix1;//,projectionMatrix;
            //projectionMatrix.perspective(60.0, 1280/728, 0.1f, 20.0f);
            viewMatrix1.lookAt(QVector3D(0,3,0),QVector3D(0,0,0),QVector3D(1,0.0,1.0));


    const QMatrix4x4 mvMat = viewMatrix1 * modelMatrix;
    const QMatrix4x4 normalMat = mvMat.inverted().transposed();
    d->shader->setUniformValue("qt_ModelViewMatrix", mvMat);
    d->shader->setUniformValue("qt_ProjectionMatrix", projectionMatrix);
    d->shader->setUniformValue("qt_NormalMatrix", normalMat);

    d->shader->setUniformValue("qt_EyePosition", QVector3D(0,0,-1));

    d->shader->setUniformValue("qt_Light.direction", QVector3D(1,1,1));
    d->shader->setUniformValue("qt_Light.ambientColor", QColor(Qt::gray));
    d->shader->setUniformValue("qt_Light.diffuseColor", QColor(Qt::white));
    d->shader->setUniformValue("qt_Light.specularColor", QColor(Qt::white));

    d->shader->setUniformValue("qt_Material.ambientColor", QColor(Qt::gray).darker());
    d->shader->setUniformValue("qt_Material.diffuseColor", QColor(Qt::gray).lighter());
    d->shader->setUniformValue("qt_Material.specularColor", QColor(Qt::white));
    d->shader->setUniformValue("qt_Material.specularPower", 0.9f);
    d->shader->setUniformValue("qt_Material.opacity", 1.0f);
    d->shader->setUniformValue("qt_Material.brightness", 2.0f);

    d->environment->bind(0);
    d->shader->setUniformValue("qt_Environment", d->environment->textureId() );

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_CCW);

    const int nrIndicies = d->torusResolution * d->tubeResolution * 6;
    glDrawElements(GL_TRIANGLES, nrIndicies, GL_UNSIGNED_INT, (void*)0);
    // glAssert(glDrawArrays(GL_TRIANGLE_STRIP, 0, nrIndicies)) ;正确的代码为什么加了 断言就出错呢？先去掉断言类
    glDisable(GL_CULL_FACE);

    d->indexBuffer->release();
    d->vertexBuffer->release();
    d->environment->release(0);
    d->shader->release();


    //绘制立方体
     //glEnable(GL_CULL_FACE);//面剔除逆时针为正
     // glCullFace(GL_CW);
    glClear(GL_COLOR_BUFFER_BIT);
    //glEnable();
    glViewport(100, 100, 300, 300);
    mCubeShaderProgram.bind();
    QMatrix4x4 _matt;
    // _matt.rotate((float) m_time->currentTime().msec(),QVector3D(1.1, 0.1, 0.1));
    //float ss= (float) m_time->currentTime().second();
    //_matt.translate(ss*10,-ss*10,ss*10);
    _matt.perspective(45,1280/728,0.1,50);
    // _matt.lookAt(QVector3D(0,0,3),QVector3D(0,0.0,0),QVector3D(0,1.0,0));//侧视图
    _matt.lookAt(QVector3D(0,3,0),QVector3D(0,0,0),QVector3D(0,0,1));//俯视图
    _matt.rotate((float) m_time->currentTime().second(),QVector3D(0, 1, 0));

    int _loca=  mCubeShaderProgram.uniformLocation("transform");
    mCubeShaderProgram.setUniformValue(_loca,_matt);

    _loca=  mCubeShaderProgram.uniformLocation("color");
    mCubeShaderProgram.setUniformValue(_loca,QVector3D(0.5,1.0f,0.7f));

    mCubVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glAssert( glDrawArrays(GL_LINE_LOOP, 0, 36) );
    //glAssert( glDrawArrays(GL_LINE_LOOP, 0, 36) );
    // mCubVAO.release();
    mCubeShaderProgram.release();

}

//新版的matrix 中含有矩阵lookat（）视图矩阵  ，透视矩阵等，所以需要自己控制矩阵，进行不同的效果
void BasicUsageScene::render()
{
    QOpenGLFunctions::initializeOpenGLFunctions();
    qDebug()<<"render";

    glClearColor(0.25, 0.35, 0.45, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    renderTorus();
    //绘制天空盒
    /*
    //glAssert( glClear(GL_COLOR_BUFFER_BIT) );
    glClearColor(0.25, 0.35, 0.45, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //glDisable(GL_CULL_FACE);
    //glDepthMask(GL_FALSE);

    // glViewport(0, 0, 1280, 728);
    m_skyBox_ShaderProgram->bind();
    m_skyBox_VertexPositionBuffer->bind();


    QMatrix4x4 _matt;
    //_matt.perspective(45,1280/728,0.1,50);
    _matt.lookAt(QVector3D(0,0,-10),QVector3D(0,0.0,0),QVector3D(0,1.0,0));//在内部需要修改

    int _loca=  m_skyBox_ShaderProgram->uniformLocation("transform");
    m_skyBox_ShaderProgram->setUniformValue(_loca,_matt);

    _cubMap->bind(0);
    _loca=  m_skyBox_ShaderProgram->uniformLocation("color");
    // m_skyBox_ShaderProgram.setUniformValue(_loca,QVector3D(0.5,1.0f,0.7f));
    m_skyBox_ShaderProgram->setUniformValue("cubemap",_cubMap->textureId());
    m_skyBox_ShaderProgram->setUniformValue("lightColor",QVector3D(0.5,0.3,0.7));

    //[]借用torus的纹理来验证 是否是着色器的原因造成图像是黑色的，或者没有绘制出来

    m_skyBox_ShaderProgram->enableAttributeArray("qt_Vertex");
    m_skyBox_ShaderProgram->setAttributeBuffer("qt_Vertex", GL_FLOAT, 0, 3);

    m_skyBox_ShaderProgram->enableAttributeArray("qt_Normal");
    m_skyBox_ShaderProgram->setAttributeBuffer("qt_Normal", GL_FLOAT, sizeof(QVector3D), 3, 2*sizeof(QVector3D));

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0, 0, -3.0);
    modelMatrix.rotate(QTime::currentTime().second(), 0, 1, 0);

   QMatrix4x4  viewMatrix,projectionMatrix;
   projectionMatrix.perspective(60.0, 1280/728, 0.1f, 20.0f);
 viewMatrix.lookAt(QVector3D(0,30,0),QVector3D(0,0,0),QVector3D(0,1.0,0));
    const QMatrix4x4 mvMat = viewMatrix * modelMatrix;
    const QMatrix4x4 normalMat = mvMat.inverted().transposed();
    m_skyBox_ShaderProgram->setUniformValue("qt_ModelViewMatrix", mvMat);
    m_skyBox_ShaderProgram->setUniformValue("qt_ProjectionMatrix", projectionMatrix);
    m_skyBox_ShaderProgram->setUniformValue("qt_NormalMatrix", normalMat);

    m_skyBox_ShaderProgram->setUniformValue("qt_EyePosition", QVector3D(0,0,-10));

    m_skyBox_ShaderProgram->setUniformValue("qt_Light.direction", QVector3D(1,1,1));
    m_skyBox_ShaderProgram->setUniformValue("qt_Light.ambientColor", QColor(Qt::red));
    m_skyBox_ShaderProgram->setUniformValue("qt_Light.diffuseColor", QColor(Qt::white));
    m_skyBox_ShaderProgram->setUniformValue("qt_Light.specularColor", QColor(Qt::white));

    m_skyBox_ShaderProgram->setUniformValue("qt_Material.ambientColor", QColor(Qt::gray).darker());
    m_skyBox_ShaderProgram->setUniformValue("qt_Material.diffuseColor", QColor(Qt::gray).lighter());
    m_skyBox_ShaderProgram->setUniformValue("qt_Material.specularColor", QColor(Qt::white));
    m_skyBox_ShaderProgram->setUniformValue("qt_Material.specularPower", 0.9f);
    m_skyBox_ShaderProgram->setUniformValue("qt_Material.opacity", 1.0f);
    m_skyBox_ShaderProgram->setUniformValue("qt_Material.brightness", 2.0f);


    m_skyBox_ShaderProgram->setUniformValue("qt_Environment", _cubMap->textureId() );

 //   glEnable(GL_CULL_FACE);
//    glCullFace(GL_CCW);

    //[]
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glAssert( glDrawArrays(GL_TRIANGLES, 0, 36) );

    m_skyBox_VertexPositionBuffer->release();
    m_skyBox_VertexPositionBuffer->release();
    m_skyBox_ShaderProgram->release();
*/

    /*

    //绘制立方体
    // glEnable(GL_CULL_FACE);//面剔除逆时针为正
    glAssert( glClear(GL_COLOR_BUFFER_BIT) );
    //glEnable();
    glViewport(0, 0, 1280, 728);
    mCubeShaderProgram.bind();
    QMatrix4x4 _matt;
    // _matt.rotate((float) m_time->currentTime().msec(),QVector3D(1.1, 0.1, 0.1));
    //float ss= (float) m_time->currentTime().second();
    //_matt.translate(ss*10,-ss*10,ss*10);
    _matt.perspective(45,1280/728,0.1,50);
    // _matt.lookAt(QVector3D(0,0,3),QVector3D(0,0.0,0),QVector3D(0,1.0,0));//侧视图
    _matt.lookAt(QVector3D(0,3,0),QVector3D(0,0,0),QVector3D(0,0,1));//俯视图
    _matt.rotate((float) m_time->currentTime().second(),QVector3D(0, 1, 0));

    int _loca=  mCubeShaderProgram.uniformLocation("transform");
    mCubeShaderProgram.setUniformValue(_loca,_matt);

    _loca=  mCubeShaderProgram.uniformLocation("color");
    mCubeShaderProgram.setUniformValue(_loca,QVector3D(0.5,1.0f,0.7f));

    mCubVAO.bind();

    glAssert( glDrawArrays(GL_LINE_LOOP, 0, 36) );
    //glAssert( glDrawArrays(GL_LINE_LOOP, 0, 36) );
    // mCubVAO.release();
    mCubeShaderProgram.release();

*/

    /*
     *
    //
    texture->bind();

    glAssert( glClear(GL_COLOR_BUFFER_BIT) );

    glViewport(0, 0, 300, 300);
    QMatrix4x4 _mat;
    _mat.rotate((float) m_time->currentTime().second(),QVector3D(1.1, 0.1, 0.1));
    // _mat.lookAt(QVector3D(0,0,1),QVector3D(0,0,0),QVector3D(0,1,0));

    mShaderProgram.bind();
    int _location=  mShaderProgram.uniformLocation("transform");
    mShaderProgram.setUniformValue(_location,_mat);
    mVAO.bind();

    glAssert( glDrawArrays(GL_TRIANGLES, 0, 4) );
    mVAO.release();
*/

    /*
    //绘制不同的视口
    glViewport(400, 400, 900, 900);
    QMatrix4x4 _mat2;
    _mat2.rotate((float)( m_time->currentTime().msec()/9),QVector3D(0.0, 0.0, 0.1));
    //_mat2.perspective()

    mShaderProgram.bind();
    _location=  mShaderProgram.uniformLocation("transform");
    mShaderProgram.setUniformValue(_location,_mat2);
    mShaderProgram.bind();
    mVAO.bind();

    glAssert( glDrawArrays(GL_TRIANGLE_STRIP, 0, 4) );
    mVAO.release();

*/


}

void BasicUsageScene::resize(int width, int height)
{

    //glAssert( glViewport(0, 0, width, height) );
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

void BasicUsageScene::prepareCubeVerTex()
{
    if (!mCubeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./cub.vert"))
    {
        qCritical() << "error";
    }
    if (!mCubeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./cub.frag"))
    {
        qCritical() << "error";
    }
    if (!mCubeShaderProgram.link())
    {
        qCritical() << "error";
    }
    glCheckError();

    float cubeVertices[] = {
        // positions
        -0.50f, -0.50f, -0.50f,
        0.50f, -0.50f, -0.500f,
        0.50f,  0.50f, -0.500f,
        0.50f,  0.50f, -0.50f,
        -0.50f,  0.50f, -0.50f,
        -0.50f, -0.50f, -0.50f,

        -0.50f, -0.50f,  0.50f,
        0.50f, -0.50f,  0.50f,
        0.50f,  0.50f,  0.50f,
        0.50f,  0.50f,  0.50f,
        -0.50f,  0.50f,  0.50f,
        -0.50f, -0.50f,  0.50f,


        -0.50f,  0.50f,  0.50f,
        -0.50f,  0.50f, -0.50f,
        -0.50f, -0.50f, -0.50f,
        -0.50f, -0.50f, -0.50f,
        -0.50f, -0.50f,  0.50f,
        -0.50f,  0.50f,  0.50f,

        0.50f,  0.50f,  0.50f,
        0.50f,  0.50f, -0.50f,
        0.50f, -0.50f, -0.50f,
        0.50f, -0.50f, -0.50f,
        0.50f, -0.50f,  0.50f,
        0.50f,  0.50f,  0.50f,

        -0.50f, -0.50f, -0.50f,
        0.50f, -0.50f, -0.50f,
        0.50f, -0.50f,  0.50f,
        0.50f, -0.50f,  0.50f,
        -0.50f, -0.50f,  0.50f,
        -0.50f, -0.50f, -0.50f,

        -0.50f,  0.50f, -0.50f,
        0.50f,  0.50f, -0.50f,
        0.50f,  0.50f,  0.50f,
        0.50f,  0.50f,  0.50f,
        -0.50f,  0.50f,  0.50f,
        -0.50f,  0.50f, -0.50f,
    };

    mCubVAO.create();
    mCubVAO.bind();

    m_cubVertexPositionBuffer.create();
    m_cubVertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_cubVertexPositionBuffer.bind();
    m_cubVertexPositionBuffer.allocate(cubeVertices,3 *6 *6 * sizeof(float));

    // mCubeShaderProgram.bind();
    m_cubVertexPositionBuffer.bind();
    mCubeShaderProgram.enableAttributeArray("vPosition");
    mCubeShaderProgram.setAttributeBuffer("vPosition", GL_FLOAT, 0, 3);
    mCubVAO.release();
    qDebug()<<mCubeShaderProgram.log();

    glCheckError();



}

#include <QDebug>
void   BasicUsageScene::cubeMap_skyBox()
{

    m_skyBox_VertexPositionBuffer=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_skyBox_ShaderProgram= new QOpenGLShaderProgram ;
    QOpenGLFunctions::initializeOpenGLFunctions();
    if (!m_skyBox_ShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./torus_vertex.glsl"))
    {
        qCritical() << "error";
    }
    if (!m_skyBox_ShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./torus_fragment.glsl"))
    {
        qCritical() << "error";
    }
    if (!m_skyBox_ShaderProgram->link())
    {
        qCritical() << "error";
    }
    qDebug()<<m_skyBox_ShaderProgram->log();

    glCheckError();


    float cubeVertices[] = {
        // positions
        -0.50f, -0.50f, -0.50f,
        0.50f, -0.50f, -0.500f,
        0.50f,  0.50f, -0.500f,
        0.50f,  0.50f, -0.50f,
        -0.50f,  0.50f, -0.50f,
        -0.50f, -0.50f, -0.50f,

        -0.50f, -0.50f,  0.50f,
        0.50f, -0.50f,  0.50f,
        0.50f,  0.50f,  0.50f,
        0.50f,  0.50f,  0.50f,
        -0.50f,  0.50f,  0.50f,
        -0.50f, -0.50f,  0.50f,


        -0.50f,  0.50f,  0.50f,
        -0.50f,  0.50f, -0.50f,
        -0.50f, -0.50f, -0.50f,
        -0.50f, -0.50f, -0.50f,
        -0.50f, -0.50f,  0.50f,
        -0.50f,  0.50f,  0.50f,

        0.50f,  0.50f,  0.50f,
        0.50f,  0.50f, -0.50f,
        0.50f, -0.50f, -0.50f,
        0.50f, -0.50f, -0.50f,
        0.50f, -0.50f,  0.50f,
        0.50f,  0.50f,  0.50f,

        -0.50f, -0.50f, -0.50f,
        0.50f, -0.50f, -0.50f,
        0.50f, -0.50f,  0.50f,
        0.50f, -0.50f,  0.50f,
        -0.50f, -0.50f,  0.50f,
        -0.50f, -0.50f, -0.50f,

        -0.50f,  0.50f, -0.50f,
        0.50f,  0.50f, -0.50f,
        0.50f,  0.50f,  0.50f,
        0.50f,  0.50f,  0.50f,
        -0.50f,  0.50f,  0.50f,
        -0.50f,  0.50f, -0.50f,
    };


    m_skyBox_VAO.create();
    m_skyBox_VAO.bind();

    m_skyBox_VertexPositionBuffer->create();
    //m_skyBox_VertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_skyBox_VertexPositionBuffer->bind();
    m_skyBox_VertexPositionBuffer->allocate(cubeVertices,3 *6 *6 * sizeof(float));

    m_skyBox_ShaderProgram->bind();
    m_skyBox_VertexPositionBuffer->bind();
    m_skyBox_ShaderProgram->enableAttributeArray("vPosition");
    m_skyBox_ShaderProgram->setAttributeBuffer("vPosition", GL_FLOAT, 0, 3);
    m_skyBox_VAO.release();





    // glEnable( GL_TEXTURE_2D);

    _bottomImage=new QImage(QImage("./sky/sky_dn.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888));
    _topImage=new QImage(QImage("./sky/sky_up.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888));
    _leftImage=new QImage(QImage("./sky/sky_lf.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888));
    _rightImage=new QImage( QImage("./sky/sky_rt.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888));
    _frontImage=new QImage( QImage("./sky/sky_ft.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888));
    _backImage=new QImage( QImage("./sky/sky_bk.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888));

    //有6 个 二维图像  组成一个cubMap
    _cubMap= new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    qDebug()<<_cubMap->faces();//qt5.2.1的面是1,而qt5.7是6个面；帮助文档也是说6个面
    _cubMap->create();
    qDebug()<<_bottomImage->width()<<_bottomImage->height()<<_bottomImage->depth();
    _cubMap->setSize(_bottomImage->width(),_bottomImage->height(),_bottomImage->depth());
    _cubMap->setFormat(QOpenGLTexture::RGBA8_UNorm);
    _cubMap->allocateStorage();


    _cubMap->setData(0,0,QOpenGLTexture::CubeMapPositiveX,
                     QOpenGLTexture::RGBA,QOpenGLTexture::UInt8,( void *)_rightImage->constBits(),0);
    _cubMap->setData(0,0,QOpenGLTexture::CubeMapNegativeX,
                     QOpenGLTexture::RGBA,QOpenGLTexture::UInt8,( void *)_leftImage->constBits(),0);

    _cubMap->setData(0,0,QOpenGLTexture::CubeMapPositiveY,
                     QOpenGLTexture::RGBA,QOpenGLTexture::UInt8,( void *)_topImage->constBits(),0);
    _cubMap->setData(0,0,QOpenGLTexture::CubeMapNegativeY,
                     QOpenGLTexture::RGBA,QOpenGLTexture::UInt8,( void *)_bottomImage->constBits(),0);


    _cubMap->setData(0,0,QOpenGLTexture::CubeMapPositiveZ,
                     QOpenGLTexture::RGBA,QOpenGLTexture::UInt8,( void *)_backImage->constBits(),0);
    _cubMap->setData(0,0,QOpenGLTexture::CubeMapNegativeZ,
                     QOpenGLTexture::RGBA,QOpenGLTexture::UInt8,( void *)_frontImage->constBits(),0);

    _cubMap->generateMipMaps();
    _cubMap->setWrapMode(QOpenGLTexture::ClampToEdge);
    _cubMap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _cubMap->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // _cubMap->release();
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    //qDebug()<< "------"<<m_skyBox_ShaderProgram.log();
    qDebug()<<_cubMap->textureId()<<_cubMap->faces();
}

void BasicUsageScene::prepareVertexBuffers()
{
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f,  10.0f, 0.0f,
        100.0f, 100.0f,0.0f
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

    //法线方向
    float normal[]=
    {
        -1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, -1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
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

    //add normal
    mVertexNormalBuffer.create();
    mVertexNormalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mVertexNormalBuffer.bind();
    mVertexNormalBuffer.allocate(normal, 4* 3 * sizeof(float));

    mVertexNormalBuffer.bind();
    mShaderProgram.enableAttributeArray("normal");
    mShaderProgram.setAttributeBuffer("normal", GL_FLOAT, 0, 2);


    mVAO.release();
    qDebug()<<mShaderProgram.log();
    glCheckError();
}

/*
           函数名： 环境光 漫反射   镜面反射
           对于ambient环境光的设置 只需要设置 片段着色器 和环境光
           */
void BasicUsageScene::prepareLight()
{
    //ambient
    int _local=   mShaderProgram.uniformLocation("lightColor");
    mShaderProgram.setUniformValue(_local,QVector3D(1.0f,1.0f,1.0f));

    //diffuse 漫反射 和光源位置 法线 片段之间的倾角有关

    _local=   mShaderProgram.uniformLocation("lightPos");
    mShaderProgram.setUniformValue(_local,QVector3D(10.0f,5.0f,10.0f));

    //special


    //我们通过反射法向量周围光的方向来计算反射向量。
    //然后我们计算反射向量和视线方向的角度差，
    //如果夹角越小，那么镜面光的影响就会越大。它的作用效果就是，
    //当我们去看光被物体所反射的那个方向的时候，我们会看到一个高光。
    _local=   mShaderProgram.uniformLocation("viewPos");
    mShaderProgram.setUniformValue(_local,QVector3D(10.0f,5.0f,-10.0f));

}


void BasicUsageScene::prepareMaterial()
{
    mShaderProgram.setUniformValue("material.ambient",QVector3D(1.0f,0.5f,0.31f));
    mShaderProgram.setUniformValue("material.diffuse",QVector3D(1.0f,0.5f,0.31f));
    mShaderProgram.setUniformValue("material.sepecuar",QVector3D(0.5f,0.5f,0.5f));
    mShaderProgram.setUniformValue("material.shininess",256.0f);


    //对于sharder不存在的变量,不影响效果,设置不起作用；浪费绑定和uniform调用
    //https://learnopengl-cn.github.io/03%20Model%20Loading/02%20Mesh/     加载网格
    /*它的Draw函数。在真正渲染这个网格之前，我们需要在调用glDrawElements函数之前先绑定相应的纹理。然而，这实际上有些困难，
                * 我们一开始并不知道这个网格（如果有的话）有多少纹理、纹理是什么类型的。所以我们该如何在着色器中设置纹理单元和采样器呢？
              为了解决这个问题，我们需要设定一个命名标准：每个漫反射纹理被命名为texture_diffuseN，每个镜面光纹理应该被命名为texture_specularN，
              其中N的范围是1到纹理采样器最大允许的数字。比如说我们对某一个网格有3个漫反射纹理，2个镜面光纹理，它们的纹理采样器应该之后会被调用
                *
                * 根据这个标准，我们可以在着色器中定义任意需要数量的纹理采样器，
                * 如果一个网格真的包含了（这么多）纹理，我们也能知道它们的名字是什么。
                * 根据这个标准，我们也能在一个网格中处理任意数量的纹理，
                * 开发者也可以自由选择需要使用的数量，他只需要定义正确的采样器就可以了（虽然定义少的话会有点浪费绑定和uniform调用）。
               */
    mShaderProgram.setUniformValue("no_exist",256.0f);
}


void BasicUsageScene::preparetorus()
{
    qDebug()<<"context:"<<this->mContext->isValid();
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-0.03125f, -0.03125f);

    QOpenGLFunctions::initializeOpenGLFunctions();

    d->vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    d->vertexBuffer->create();
    d->vertexBuffer->bind();
    d->vertexBuffer->allocate(d->tubeResolution * d->torusResolution * sizeof(QVector3D) * 2);

    d->indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    d->indexBuffer->create();
    d->indexBuffer->bind();
    d->indexBuffer->allocate( d->torusResolution * d->tubeResolution * 6 * sizeof(uint) );

    QMatrix4x4 matrix;
    const float tubeAngleStep = 360.0f / float(d->tubeResolution-1);
    const float torusAngleStep = 360.0f / float(d->torusResolution-1);

    int vertexBufferOffset = 0;
    for(int i=0; i<d->torusResolution; i++)
    {
        QMatrix4x4 tmp = matrix;
        tmp.translate(d->torusRadius, 0, 0);

        const QVector3D centerPt = tmp.map(QVector3D(0,0,0));
        for(int j=0; j<d->tubeResolution; j++)
        {
            const QVector3D pt = tmp.map( QVector3D(d->tubeRadius, 0, 0) );
            d->vertexBuffer->write(vertexBufferOffset, &pt, sizeof(pt));
            vertexBufferOffset += sizeof(pt);

            const QVector3D normal = (pt - centerPt).normalized();

            d->vertexBuffer->write(vertexBufferOffset, &normal, sizeof(normal));
            vertexBufferOffset += sizeof(normal);
            tmp.rotate(tubeAngleStep, QVector3D(0,1,0));
        }

        matrix.rotate(torusAngleStep, QVector3D(0,0,1));
    }

    int indexBufferOffset = 0;
    int base = 0;
    const int nrPoints = (d->torusResolution * d->tubeResolution);
#define P(x) (x)%nrPoints
    for(int i=0; i<d->torusResolution; i++)
    {
        for(int j=0; j<d->tubeResolution; j++)
        {
            const uint tgls[] = {
                P(base),
                P(base+1),
                P(base+1+d->tubeResolution),
                P(base),
                P(base+1+d->tubeResolution),
                P(base+d->tubeResolution)
            };
            d->indexBuffer->write(indexBufferOffset, (const void*)tgls, sizeof(uint)*6);
            indexBufferOffset += sizeof(uint)*6;
            ++base;
        }
#undef P
    }

    d->indexBuffer->release();
    d->vertexBuffer->release();
    qDebug()<<"torus set Data begin";
    // Load all environment texture images
    const QImage posx = QImage("./images/sky_dn.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888);
    const QImage posy = QImage("./images/sky_up.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888);
    const QImage posz = QImage("./images/sky_lf.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888);
    const QImage negx = QImage("./images/sky_rt.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888);
    const QImage negy = QImage("./images/sky_ft.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888);
    const QImage negz = QImage("./images/sky_bk.jpg").mirrored().convertToFormat(QImage::Format_RGBA8888);

    // Construct the environment texture
    d->environment = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    qDebug()<<"facesNum:"<<d->environment->faces();
    d->environment->create();
    d->environment->setSize(posx.width(), posx.height(), posx.depth());
    d->environment->setFormat(QOpenGLTexture::RGBA8_UNorm);
    d->environment->allocateStorage();
    d->environment->setData(0, 0, QOpenGLTexture::CubeMapPositiveX,
                            QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            (const void*)posx.constBits(), 0);
    d->environment->setData(0, 0, QOpenGLTexture::CubeMapPositiveY,
                            QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            (const void*)posy.constBits(), 0);
    d->environment->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,
                            QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            (const void*)posz.constBits(), 0);
    d->environment->setData(0, 0, QOpenGLTexture::CubeMapNegativeX,
                            QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            (const void*)negx.constBits(), 0);
    d->environment->setData(0, 0, QOpenGLTexture::CubeMapNegativeY,
                            QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            (const void*)negy.constBits(), 0);
    d->environment->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,
                            QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            (const void*)negz.constBits(), 0);
    d->environment->generateMipMaps();
    d->environment->setWrapMode(QOpenGLTexture::ClampToEdge);
    d->environment->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    d->environment->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    qDebug()<<"facesNum:"<<d->environment->faces();
    // Load and link shader programs
    d->shader = new QOpenGLShaderProgram;
    d->shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./torus_vertex.glsl");
    d->shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./torus_fragment.glsl");
    d->shader->link();

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


    QOpenGLTexture *_texTest  = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    qDebug()<<_texTest->faces();
}
#include <QTime>
void BasicUsageScene::renderTorus()
{
    //preparetorus();

    d->shader->bind();
    d->vertexBuffer->bind();
    d->indexBuffer->bind();

    d->shader->enableAttributeArray("qt_Vertex");
    d->shader->setAttributeBuffer("qt_Vertex", GL_FLOAT, 0, 3, 2*sizeof(QVector3D));

    d->shader->enableAttributeArray("qt_Normal");
    d->shader->setAttributeBuffer("qt_Normal", GL_FLOAT, sizeof(QVector3D), 3, 2*sizeof(QVector3D));

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0, 0, -3.0);
    modelMatrix.rotate(d->angle, 0, 1, 0);

    d->angle += 5;

    QMatrix4x4  viewMatrix,projectionMatrix;
    projectionMatrix.perspective(60.0, 1280/728, 0.1f, 2000.0f);
    viewMatrix.lookAt(QVector3D(0,10*sin(QTime::currentTime().second()),0),QVector3D(0,0,0),QVector3D(1.0,1.0,1.0));
    //viewMatrix.setToIdentity();

    const QMatrix4x4 mvMat = viewMatrix * modelMatrix;
    const QMatrix4x4 normalMat = mvMat.inverted().transposed();
    d->shader->setUniformValue("qt_ModelViewMatrix", mvMat);
    d->shader->setUniformValue("qt_ProjectionMatrix", projectionMatrix);
    d->shader->setUniformValue("qt_NormalMatrix", normalMat);

    d->shader->setUniformValue("qt_EyePosition", QVector3D(0,0,-1));

    d->shader->setUniformValue("qt_Light.direction", QVector3D(1,1,1));
    d->shader->setUniformValue("qt_Light.ambientColor", QColor(Qt::gray));
    d->shader->setUniformValue("qt_Light.diffuseColor", QColor(Qt::white));
    d->shader->setUniformValue("qt_Light.specularColor", QColor(Qt::white));

    d->shader->setUniformValue("qt_Material.ambientColor", QColor(Qt::gray).darker());
    d->shader->setUniformValue("qt_Material.diffuseColor", QColor(Qt::gray).lighter());
    d->shader->setUniformValue("qt_Material.specularColor", QColor(Qt::white));
    d->shader->setUniformValue("qt_Material.specularPower", 0.9f);
    d->shader->setUniformValue("qt_Material.opacity", 1.0f);
    d->shader->setUniformValue("qt_Material.brightness", 2.0f);

    d->environment->bind(0);
    d->shader->setUniformValue("qt_Environment", d->environment->textureId() );

    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);

    const int nrIndicies = d->torusResolution * d->tubeResolution * 6;
    glDrawElements(GL_TRIANGLES, nrIndicies, GL_UNSIGNED_INT, (void*)0);
    // glAssert(glDrawArrays(GL_TRIANGLE_STRIP, 0, nrIndicies)) ;正确的代码为什么加了 断言就出错呢？先去掉断言类
    glDisable(GL_CULL_FACE);

    d->indexBuffer->release();
    d->vertexBuffer->release();
    d->environment->release(0);
    d->shader->release();
}




/*https://forum.qt.io/topic/53605/cubemap-texture-with-qopengltexture-class
 *
    texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    texture->create();
    texture->bind();
    texture->setSize(256, 256);
    texture->setFormat(QOpenGLTexture::RGBAFormat);
    texture->allocateStorage();
    texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);

    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*) x.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*) y.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*) z.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*) _x.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*) _y.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*) _z.bits());
 *
And the corresponding bindings in the draw function:

//    glBindTexture(GL_TEXTURE_CUBE_MAP, textureGL);
texture->bind();

*/
