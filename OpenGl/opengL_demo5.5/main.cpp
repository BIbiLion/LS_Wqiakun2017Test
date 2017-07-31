/*QOpenGLWidget is begin from 5.4 ,so before version is not ok.
*/


//在之前的基础上继续分析openglwindow例子——了解视角
//http://www.bubuko.com/infodetail-1533025.html

//#include "openglwindow.h"

#include <QtGui/QGuiApplication>

#include <QtGui/QMatrix4x4>

#include <QtGui/QOpenGLShaderProgram>

#include <QtGui/QScreen>

#include <QtCore/qmath.h>



//! [1]
#include <QOpenGLWidget>

class TriangleWindow : public QOpenGLWidget//OpenGLWindow

{

public:

    TriangleWindow();

    void initialize();// Q_DECL_OVERRIDE;

    void render();// Q_DECL_OVERRIDE;

private:

    GLuint m_posAttr;

    GLuint m_colAttr;

    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;

    int m_frame;

};

TriangleWindow::TriangleWindow()

    : m_program(0)

    , m_frame(0)

{

}

//! [1]

//! [2]
//!
//! 片段着色器，以顶点着色器的颜色输出做输入，然后直接输出。
//! 顶点着色器，输入的顶点颜色做输出，输入一个常量矩阵与位置属性，输出相乘后的位置结果。
//!
//!
//!
#include <QSurfaceFormat>
int main(int argc, char **argv)

{

    QGuiApplication app(argc, argv);

    QSurfaceFormat format;

    format.setSamples(16);

    TriangleWindow window;

    window.setFormat(format);

    window.resize(640, 480);

    window.show();

    window.setAnimating(true);

    return app.exec();

}

//! [2]

//! [3]

static const char *vertexShaderSource =

        "attribute highp vec4 posAttr;\n"

        "attribute lowp vec4 colAttr;\n"

        "varying lowp vec4 col;\n"

        "uniform highp mat4 matrix;\n"

        "void main() {\n"

        "   col = colAttr;\n"

        "   gl_Position = matrix * posAttr;\n"

        "}\n";

static const char *fragmentShaderSource =

        "varying lowp vec4 col;\n"

        "void main() {\n"

        "   gl_FragColor = col;\n"

        "}\n";

//! [3]

//! [4] initialize初始化，link之后，获取获取三个属性的位置，没有初始化。

void TriangleWindow::initialize()

{

    m_program = new QOpenGLShaderProgram(this);

    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);

    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    m_program->link();

    m_posAttr = m_program->attributeLocation("posAttr");

    m_colAttr = m_program->attributeLocation("colAttr");

    m_matrixUniform = m_program->uniformLocation("matrix");

}

//! [4]

//! [5]


void TriangleWindow::render()

{


    //  获取设备的像素比
    const qreal retinaScale = devicePixelRatio();

    //   获得像素比后，按像素比设置视窗大小。
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;

    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);

    matrix.translate(0, 0, -2);

    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {

        0.0f, 0.707f,

        -0.5f, -0.5f,

        0.5f, -0.5f

    };

    GLfloat colors[] = {

        1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 1.0f

    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);

    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;

}


