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
 * ┃ file name: GLWidget.cpp                                                ┃
 * ┃ create date: 2013年8月19日星期一 23时34分9秒                           ┃
 * ┃ last modified date: 2013年8月22日星期四 22时46分0秒                    ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include <cmath>
#include <QMessageBox>
#include <QWheelEvent>
#include "GLWidget.h"

/*---------------------------------------------------------------------------*/
extern QString g_VertexShaderText;
extern QString g_FragmentShaderText;
/*---------------------------------------------------------------------------*/
void MyPerspective( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar )
{
    using namespace std;
    // 使用OpenGL函数，但是需要添加math.h头文件
    GLdouble rFov = fov * 3.14159265 / 180.0;
    GLdouble up = -zNear * tan( rFov / 2.0 );
    GLdouble down = -up;
    GLdouble left = up * aspectRatio;
    GLdouble right = -left;
    glFrustum( left, right, up, down, zNear, zFar );
}
/*---------------------------------------------------------------------------*/
void PopupError( const QString& info )
{
    QString header = QObject::tr( "Sorry but there are some errors in using shaders.\n"
                                  "Here are some useful information:\n" );
    QMessageBox::warning(
                Q_NULLPTR,
                QObject::tr( "Shader program error" ),
                header + info );
}
#define CHECK( stmt, info ) \
    if ( !stmt )\
{\
    PopupError( info );\
    return;\
    }
/*---------------------------------------------------------------------------*/
GLWidget::GLWidget( QWidget* pParent ):
    QGLWidget( QGLFormat( QGL::SampleBuffers ), pParent )
{
    resize( 640, 360 );
    m_AspectRatio = qreal( width( ) ) / qreal( height( ) ); // 初始的宽高比
}
/*---------------------------------------------------------------------------*/
void GLWidget::resizeGL( int width, int height )
{
    // 改变大小时程序如何应对？
    GLdouble aspectRatio = GLdouble( width ) / GLdouble( height );

    // 设置视口
    if ( aspectRatio < m_AspectRatio )
    {
        GLint smallHeight = GLint( GLdouble( width ) / m_AspectRatio );
        GLint heightBlank = ( GLint( height ) - smallHeight ) / 2;
        glViewport( 0, heightBlank, GLint( width ), smallHeight );
       // glViewport( 0, 0, 500, 500);
    }
    else
    {
        GLint smallWidth = GLint( GLdouble( height ) * m_AspectRatio );
        GLint widthBlank = ( GLint( width ) - smallWidth ) / 2;
        glViewport( widthBlank, 0, smallWidth, GLint( height ) );
        // glViewport( 0, 0, 500, 500);
    }
     glViewport( 200, 200, 500, 500);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    // 设置裁剪区域
    MyPerspective( 45, m_AspectRatio, 0.5, 1000.0 );

    // 为模型视图载入标准矩阵
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glRotatef(30,0,0,1);
}
/*---------------------------------------------------------------------------*/
void GLWidget::initializeGL( void )
{
    initializeOpenGLFunctions( );

    // 打开开关
    qglClearColor( QColor( 255, 255, 255, 255 ) );
    qglColor( QColor( 255, 0, 0 ) );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glEnable( GL_DEPTH_TEST );
    glFrontFace( GL_CW );
    glEnable( GL_CULL_FACE );

    // 创建顶点缓存并且设置顶点
    glGenBuffers( 1, &m_VertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_VertexBuffer );
    CubeVertices( 20.0f );

    // 创建颜色缓存以及设置颜色
    glGenBuffers( 1, &m_ColorBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_ColorBuffer );
    CubeColors( );

    SetupShaders( );// 设置着色器

    // 设置摄像机
    m_Camera.SetPos( QVector3D( 0.0, 0.0, 20.0 ) );// 设置摄像机坐标
}
/*---------------------------------------------------------------------------*/
void GLWidget::paintGL( void )
{
    static GLubyte cubeIndices[] =
    {
        0, 1, 2, 0, 2, 3,// 下面
        7, 6, 4, 6, 5, 4,// 上面
        7, 4, 3, 4, 0, 3,// 左面
        5, 6, 1, 6, 2, 1,// 右面
        4, 5, 0, 5, 1, 0,// 前面
        3, 2, 6, 3, 6, 7,// 背面
    };

    glClear( GL_COLOR_BUFFER_BIT |
             GL_DEPTH_BUFFER_BIT );

    glPushMatrix( );
    m_Camera.Apply( );


    // 绑定顶点和颜色缓存，以及绘制
    glBindBuffer( GL_ARRAY_BUFFER, m_VertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_ColorBuffer );
    glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices );

    glPopMatrix( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::CompileAndLinkVertexShader(const QString& shaderText )
{
    m_ShaderProgram.release( );
    m_ShaderProgram.removeShader( m_pVertexShader );
    delete m_pVertexShader;

    m_pVertexShader = new QOpenGLShader( QOpenGLShader::Vertex,
                                         &m_ShaderProgram );
    CHECK( m_pVertexShader->compileSourceCode( shaderText ),
           m_pVertexShader->log( ) );

    CHECK( m_ShaderProgram.addShader( m_pVertexShader ),
           m_ShaderProgram.log( ) );

    CHECK( m_ShaderProgram.link( ),
           m_ShaderProgram.log( ) );

    m_ShaderProgram.bind( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::CompileAndLinkFragmentShader(const QString &shaderText)
{
    m_ShaderProgram.release( );
    m_ShaderProgram.removeShader( m_pFragmentShader );
    delete m_pFragmentShader;

    m_pFragmentShader = new QOpenGLShader( QOpenGLShader::Fragment,
                                           &m_ShaderProgram );
    CHECK( m_pFragmentShader->compileSourceCode( shaderText ),
           m_pFragmentShader->log( ) );

    CHECK( m_ShaderProgram.addShader( m_pFragmentShader ),
           m_ShaderProgram.log( ) );

    CHECK( m_ShaderProgram.link( ),
           m_ShaderProgram.log( ) );

    m_ShaderProgram.bind( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::SetupShaders( void )
{
    m_pVertexShader = new QOpenGLShader( QOpenGLShader::Vertex,
                                         &m_ShaderProgram );
    CHECK( m_pVertexShader->compileSourceCode( g_VertexShaderText ),
           m_pVertexShader->log( ) );

    m_pFragmentShader = new QOpenGLShader( QOpenGLShader::Fragment,
                                           &m_ShaderProgram );
    CHECK( m_pFragmentShader->compileSourceCode( g_FragmentShaderText ),
           m_pFragmentShader->log( ) );

    CHECK( m_ShaderProgram.addShader( m_pVertexShader ),
           m_ShaderProgram.log( ) );
    CHECK( m_ShaderProgram.addShader( m_pFragmentShader ),
           m_ShaderProgram.log( ) );

    CHECK( m_ShaderProgram.link( ),
           m_ShaderProgram.log( ) );

    m_ShaderProgram.bind( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::CubeVertices( GLfloat length )
{
    // 设置立方体的顶点
    GLfloat semiLength = length / 2.0f;
    GLfloat cubeVertices[] =
    {
        -semiLength, -semiLength, semiLength,   // 0
        semiLength, -semiLength, semiLength,    // 1
        semiLength, -semiLength, -semiLength,   // 2
        -semiLength, -semiLength, -semiLength,  // 3
        -semiLength, semiLength, semiLength,    // 4
        semiLength, semiLength, semiLength,     // 5
        semiLength, semiLength, -semiLength,    // 6
        -semiLength, semiLength, -semiLength,   // 7
    };

    glBufferData( GL_ARRAY_BUFFER, sizeof( cubeVertices ),
                  cubeVertices, GL_STATIC_DRAW );
    glVertexPointer( 3, GL_FLOAT, 0, 0 );
}
/*---------------------------------------------------------------------------*/
void GLWidget::CubeColors( void )
{
    GLubyte cubeColors[] =
    {
        255, 0, 0, 255,
        255, 255, 0, 255,
        0, 255, 0, 255,
        0, 0, 0, 255,
        255, 0, 255, 255,
        255, 255, 255, 255,
        0, 255, 255, 255,
        0, 0, 255, 255,
    };

    glBufferData( GL_ARRAY_BUFFER, sizeof( cubeColors ),
                  cubeColors, GL_STATIC_DRAW );
    glColorPointer( 4, GL_UNSIGNED_BYTE, 0, 0 );
}
/*---------------------------------------------------------------------------*/
void GLWidget::mousePressEvent( QMouseEvent* pEvent )
{
    m_LastPos = pEvent->pos( );
    switch ( pEvent->button( ) )
    {
    case Qt::LeftButton:
    case Qt::RightButton:

        break;
    case Qt::MiddleButton:
    default: break;
    }
}
/*---------------------------------------------------------------------------*/
void GLWidget::mouseMoveEvent( QMouseEvent* pEvent )
{
    QVector2D delta( pEvent->pos( ) - m_LastPos );
    if ( pEvent->buttons( ) & Qt::LeftButton ||
         pEvent->buttons( ) & Qt::RightButton )
    {
        m_Camera.SetRotateH( -delta.y( ), true );
        m_Camera.SetRotateY( delta.x( ), true );
    }
    else if ( pEvent->buttons( ) & Qt::MiddleButton )
    {
        m_Camera.SetPos( QVector3D( -delta.x( ) / 5, delta.y( ) / 5, 0 ), true );
    }
    repaint( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::mouseReleaseEvent( QMouseEvent* pEvent )
{
    QVector2D delta( pEvent->pos( ) - m_LastPos );
    switch ( pEvent->button( ) )
    {
    case Qt::LeftButton:
    case Qt::RightButton:
        m_Camera.SetRotateH( -delta.y( ) );
        m_Camera.SetRotateY( delta.x( ) );
        break;
    case Qt::MiddleButton:
        m_Camera.SetPos( QVector3D( -delta.x( ) / 5, delta.y( ) / 5, 0 ) );
        break;
    default: break;
    }
    repaint( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::wheelEvent( QWheelEvent* pEvent )
{
    m_Camera.ZoomIn( pEvent->delta( ) / 10 );
    repaint( );
}
/*---------------------------------------------------------------------------*/
