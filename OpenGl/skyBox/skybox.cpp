#include "skybox.h"

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QOpenGLFunctions>

SkyBox::SkyBox()
{
    d = new SkyBoxData;
}

SkyBox::~SkyBox()
{
    delete d;
}

void SkyBox::render(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix)
{
    this->init();

    d->shader->bind();
    d->vertexBuffer->bind();
    d->indexBuffer->bind();

    d->shader->enableAttributeArray("qt_Vertex");
    d->shader->setAttributeBuffer("qt_Vertex", GL_FLOAT, 0, 3, 0);

    d->shader->enableAttributeArray("qt_MultiTexCoord0");
    const int texCoordsOffset = 24 * sizeof(QVector3D);
    d->shader->setAttributeBuffer("qt_MultiTexCoord0", GL_FLOAT, texCoordsOffset, 2, 0);

    d->matrix.rotate(-1, 0, 1, 0);

    QMatrix4x4 modelMatrix = d->matrix;
    modelMatrix.scale(10, 10, 10);

    const QMatrix4x4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    d->shader->setUniformValue("qt_ModelViewProjectionMatrix", mvp);

    for(int i=0; i<6; i++)
    {
        d->textures[i]->bind(i+1);
        d->shader->setUniformValue("qt_Texture0", (i+1));

        const uint triangleOffset = i*6*sizeof(uint);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)triangleOffset);

        d->textures[i]->release(i+1);
    }

    d->indexBuffer->release();
    d->vertexBuffer->release();
    d->shader->release();
}

void SkyBox::init()
{
    if(d->shader)
        return;

    QOpenGLFunctions::initializeOpenGLFunctions();
qDebug()<<"skyBox setData begin";
    // Load all texture images
    const QImage posx = QImage("./images/sky_rt.jpg").mirrored();
    const QImage posy = QImage("./images/sky_up.jpg").mirrored();
    const QImage posz = QImage("./images/sky_ft.jpg").mirrored();
    const QImage negx = QImage("./images/sky_lf.jpg").mirrored();
    const QImage negy = QImage("./images/sky_dn.jpg").mirrored();
    const QImage negz = QImage("./images/sky_bk.jpg").mirrored();
qDebug()<<"skyBox setData end";
    // Load images as independent texture objects
    d->textures[0] = new QOpenGLTexture(posx);
    d->textures[1] = new QOpenGLTexture(posy);
    d->textures[2] = new QOpenGLTexture(posz);
    d->textures[3] = new QOpenGLTexture(negx);
    d->textures[4] = new QOpenGLTexture(negy);
    d->textures[5] = new QOpenGLTexture(negz);
    for(int i=0; i<6; i++)
    {
        d->textures[i]->setWrapMode(QOpenGLTexture::ClampToEdge);
        d->textures[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        d->textures[i]->setMagnificationFilter(QOpenGLTexture::Linear);
    }

    // Construct a template square of size 2x2
    const QVector3D p1(-1, 1, 0); // top-left
    const QVector3D p2(-1, -1, 0); // bottom-left
    const QVector3D p3(1, -1, 0); // bottom-right
    const QVector3D p4(1, 1, 0); // top-right

    // Array for storing geometry of the cube
    QVector<QVector3D> geometry;
    geometry.reserve(24);

    // Transform p1 ... p4 for posx
    QMatrix4x4 mat;
    mat.translate(1, 0, 0);
    mat.rotate(-90, 0, 1, 0);
    geometry << mat.map(p1) << mat.map(p2)
             << mat.map(p3) << mat.map(p4);

    // Transform p1 ... p4 for posy
    mat.setToIdentity();
    mat.translate(0, 1, 0);
    mat.rotate(90, 1, 0, 0);
    geometry << mat.map(p1) << mat.map(p2)
             << mat.map(p3) << mat.map(p4);

    // Transform p2 ... p4 for posz
    mat.setToIdentity();
    mat.translate(0, 0, -1);
    geometry << mat.map(p1) << mat.map(p2)
             << mat.map(p3) << mat.map(p4);

    // Transform p2 ... p4 for negx
    mat.setToIdentity();
    mat.translate(-1, 0, 0);
    mat.rotate(90, 0, 1, 0);
    geometry << mat.map(p1) << mat.map(p2)
             << mat.map(p3) << mat.map(p4);

    // Transform p2 ... p4 for negy
    mat.setToIdentity();
    mat.translate(0, -1, 0);
    mat.rotate(-90, 1, 0, 0);
    geometry << mat.map(p1) << mat.map(p2)
             << mat.map(p3) << mat.map(p4);

    // Transform p2 ... p4 for negz
    mat.setToIdentity();
    mat.translate(0, 0, 1);
    mat.rotate(180, 0, 1, 0);
    geometry << mat.map(p1) << mat.map(p2)
             << mat.map(p3) << mat.map(p4);

    // Texture coordinates
    QVector<QVector2D> texCoords;
    texCoords.reserve(24);
    for(int i=0; i<6; i++)
    {
        texCoords << QVector2D(0, 1)
                  << QVector2D(0, 0)
                  << QVector2D(1, 0)
                  << QVector2D(1, 1);
    }

    // Triangles
    QVector<uint> triangles;
    triangles.reserve(36);
    for(int i=0; i<6; i++)
    {
        const int base = i*4;
        triangles << base << base+1 << base+2;
        triangles << base << base+2 << base+3;
    }

    // Store the arrays in buffers
    d->vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    d->vertexBuffer->create();
    d->vertexBuffer->bind();
    d->vertexBuffer->allocate( geometry.size()*sizeof(QVector3D) +
                               texCoords.size()*sizeof(QVector2D) );
    d->vertexBuffer->write(0, (const void *)geometry.constData(),
                           geometry.size()*sizeof(QVector3D) );
    d->vertexBuffer->write(geometry.size()*sizeof(QVector3D),
                           (const void *)texCoords.constData(),
                           texCoords.size()*sizeof(QVector2D) );
    d->vertexBuffer->release();

    d->indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    d->indexBuffer->create();
    d->indexBuffer->bind();
    d->indexBuffer->allocate((const void*)triangles.constData(),
                             triangles.size()*sizeof(uint));
    d->indexBuffer->release();

    // Create shaders
    d->shader = new QOpenGLShaderProgram;
    d->shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./skybox_vertex.glsl");
    d->shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./skybox_fragment.glsl");
    d->shader->link();
}
