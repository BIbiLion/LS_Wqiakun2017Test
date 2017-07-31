#ifndef SKYBOX_H
#define SKYBOX_H
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

struct SkyBoxData
{
    SkyBoxData() {
        for(int i=0; i<6; i++)
            this->textures[i] = 0;
        this->vertexBuffer = 0;
        this->indexBuffer = 0;
        this->shader = 0;
    }
    ~SkyBoxData() {
        delete this->shader;
        delete this->indexBuffer;
        delete this->vertexBuffer;
        for(int i=5; i>=0; i--)
            delete this->textures[i];
    }

    QOpenGLTexture *textures[6];
    QOpenGLBuffer *vertexBuffer;
    QOpenGLBuffer *indexBuffer;
    QOpenGLShaderProgram *shader;
    QMatrix4x4 matrix;
};
#include <QOpenGLFunctions>
class SkyBox :public QOpenGLFunctions
{
public:
    SkyBox();
    ~SkyBox();
    void render(const QMatrix4x4 &viewMatrix,const QMatrix4x4 &projectionMatrix);
    void init();
private:
    SkyBoxData *d;
};

#endif // SKYBOX_H
