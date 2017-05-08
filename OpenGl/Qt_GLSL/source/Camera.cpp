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
 * ┃ file name: Camera.cpp                                                  ┃
 * ┃ create date: 2013年8月2日星期五 14时12分49秒                           ┃
 * ┃ last modified date: 2013年8月22日星期四 22时46分0秒                    ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include <QtOpenGL>
#include "Camera.h"
/*---------------------------------------------------------------------------*/
Camera::Camera( void )
{
    m_Pos = QVector3D( 0, 0, 0 );
    m_RotateY = 0.0;
    m_RotateH = 0.0;
    m_Len = QVector3D( 0, 0, 0 );
    m_TryPos = QVector3D( 0, 0, 0 );
    m_TryRotateY = 0.0;
    m_TryRotateH = 0.0;
    m_Try = false;
}
/*---------------------------------------------------------------------------*/
void Camera::SetPos( const QVector3D pos, bool _try )
{
    m_Try = _try;

    if ( m_Try ) m_TryPos = pos;
    else
    {
        m_Pos += pos;
        m_Offset2D += QVector3D( pos.x( ), pos.y( ), 0 );
        m_TryPos = QVector3D( 0, 0, 0 );
    }
}

void Camera::SetRotateY( float rotateY, bool _try )
{
    m_Try = _try;

    if ( m_Try ) m_TryRotateY = rotateY;
    else
    {
        m_RotateY += rotateY;
        m_TryRotateY = 0.0;
    }
}

void Camera::SetRotateH( float rotateH, bool _try )
{
    m_Try = _try;

    if ( m_Try ) m_TryRotateH = rotateH;
    else
    {
        m_RotateH += rotateH;
        m_TryRotateH = 0.0;
    }
}
/*---------------------------------------------------------------------------*/
void Camera::Apply( void )
{
    QVector3D ha = QVector3D::crossProduct( m_Pos -
                                            QVector3D( 0, m_Pos.y( ), 0 ) -
                                            m_Offset2D,
                                            QVector3D( 0, 1, 0 ) );
    QVector3D focus = -m_Pos - m_TryPos - m_Len;
    glTranslatef( focus.x( ), focus.y( ), focus.z( ) );
    glRotatef( m_RotateH + m_TryRotateH, ha.x( ), ha.y( ), ha.z( ) );
    glRotatef( m_RotateY + m_TryRotateY, 0, 1, 0 );
}
/*---------------------------------------------------------------------------*/
void Camera::ZoomIn( float step )
{
    QVector3D tangent = m_Pos - QVector3D( 0, m_Pos.y( ), 0 );
    m_Len -= tangent.normalized( ) * step;
}

