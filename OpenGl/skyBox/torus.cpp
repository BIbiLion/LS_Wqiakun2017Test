#include "torus.h"

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>



Torus::Torus()
{
    d = new TorusData;
}

Torus::~Torus()
{
    delete d;
}
#include "./glassert.h"
void Torus::render(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix)
{
    this->init();

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

void Torus::init()
{
    if(d->shader)
        return;

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

