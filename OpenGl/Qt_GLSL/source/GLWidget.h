/* ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 * ┃     ┏━┣━┣┓　　┏┓┏┓┳┓┏━━┓┣┣━┓　┓　┓┣┳━┓       ┃
 * ┃     ┏┏┏╯━┓┳┳━┛┏╯┃┃┃　　┃┣┣━┓┃┃　┃┃┃　　       ┃
 * ┃     ┃┃┏━╮┃┗┗┏╯┗┃┃╯┃　　┃┏┣━┓┃┃　┃╯┣━┓       ┃
 * ┃     ╰┫┏━┻╯┗┳┣┛┏┛┃┃┣━━┫┃┃　┃┃┃┗╯　┃　　       ┃
 * ┃     ┏┫━┳━┫┏┃┣┓┗┃┃╯┃　　┃┃┃　┃　┃　┃　┣━┓       ┃
 * ┃     ┗┗┗━━╯┗┛┛╯┗╯╰　┗━━╯　┛　┛┗╯　╰┛┗　　       ┃
 * ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 * ┃                     Copyright (c) 2013 jiangcaiyang                    ┃
 * ┃ This software is provided 'as-is', without any express or implied      ┃
 * ┃ warranty. In no event will the authors be held liable for any damages  ┃
 * ┃ arising from the use of this software.                                 ┃
 * ┃                                                                        ┃
 * ┃ Permission is granted to anyone to use this software for any purpose,  ┃
 * ┃ including commercial applications, and to alter it and redistribute it ┃
 * ┃ freely, subject to the following restrictions:                         ┃
 * ┃                                                                        ┃
 * ┃ 1. The origin of this software must not be misrepresented; you must    ┃
 * ┃    not claim that you wrote the original software. If you use this     ┃
 * ┃    software in a product, an acknowledgment in the product             ┃
 * ┃    documentation would be appreciated but is not required.             ┃
 * ┃ 2. Altered source versions must be plainly marked as such, and must    ┃
 * ┃    not be misrepresented as being the original software.               ┃
 * ┃ 3. This notice may not be removed or altered from any source           ┃
 * ┃    distribution.                                                       ┃
 * ┃                                                                        ┃
 * ┃ jiangcaiyang jiangcaiyang123@163.com                                   ┃
 * ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 * ┃ file name: GLWidget.h                                                  ┃
 * ┃ create date: 2013年8月19日星期一 23时14分10秒                          ┃
 * ┃ last modified date: 2013年8月22日星期四 22时46分0秒                    ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QPoint>
#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "Camera.h"

class GLWidget: public QGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLWidget( QWidget* pParent = 0 );

    void initializeGL( void );
    void resizeGL( int width, int height );
    void paintGL( void );
public slots:
    void CompileAndLinkVertexShader( const QString& shaderText );
    void CompileAndLinkFragmentShader( const QString& shaderText );
protected:
    void mousePressEvent( QMouseEvent* pEvent );
    void mouseReleaseEvent( QMouseEvent* pEvent );
    void mouseMoveEvent( QMouseEvent* pEvent );
    void wheelEvent( QWheelEvent* pEvent );

private:
    void CubeVertices( GLfloat length );
    void CubeColors( void );
    void SetupShaders( void );

    // 摄像机
    Camera          m_Camera;

    // 鼠标事件需要
    QPoint          m_LastPos;

    // 顶点缓存和颜色缓存
    GLuint          m_VertexBuffer, m_ColorBuffer;

    // 着色器
    QOpenGLShader               *m_pVertexShader, *m_pFragmentShader;

    // 着色器程序
    QOpenGLShaderProgram        m_ShaderProgram;

    // 维持宽高比的参数
    qreal           m_AspectRatio;
};

#endif // GLWIDGET_H
