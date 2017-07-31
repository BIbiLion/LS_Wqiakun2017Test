#ifndef TORUS_H
#define TORUS_H
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class TorusData;
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
#include <QOpenGLFunctions>
class Torus:public QOpenGLFunctions
{
public:
    Torus();
    ~Torus();
    void render(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix);
    void init();
private:
    TorusData *d;
};

#endif // TORUS_H
