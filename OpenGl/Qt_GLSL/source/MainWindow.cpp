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
 * ┃ file name: MainWindow.cpp                                              ┃
 * ┃ create date: 2013年8月24日星期六 22时39分15秒                          ┃
 * ┃ last modified date: 2013年8月24日星期六 22时39分15秒                   ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include <QApplication>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFile>
#include <QMessageBox>
#include "MainWindow.h"
#include "GLWidget.h"
#include "ShaderDialog.h"
#include "Feedback/FeedbackDialog.h"
/*---------------------------------------------------------------------------*/
QString GetShaderFromFile( const QString& fileName )
{
    QString retStr;
    QFile file( fileName );
    if ( file.open( QIODevice::ReadOnly ) )
    {
        retStr = file.readAll( );
        file.close( );
    }
    return retStr;
}
/*---------------------------------------------------------------------------*/
QString g_VertexShaderText = GetShaderFromFile( ":/Shader/DefaultShader.vert" );
QString g_FragmentShaderText = GetShaderFromFile( ":/Shader/DefaultShader.frag" );
/*---------------------------------------------------------------------------*/
MainWindow::MainWindow( QWidget* pParent ):
    QMainWindow( pParent )
{
    setWindowTitle( tr( "Shader Example" ) );

    GLWidget* pWidget = new GLWidget( this );
    setCentralWidget( pWidget );

    // 设置子窗口
    SetupMenu( );
    resize( pWidget->width( ),
            pWidget->height( ) + menuBar( )->height( ) * 2 / 3 );
}
/*---------------------------------------------------------------------------*/
void MainWindow::SetupMenu( void )
{
    QMenu* pOption = new QMenu( tr( "Option" ), this );
    QMenu* pAbout = new QMenu( tr( "About" ), this );

    pOption->addAction(
                QIcon( ),
                tr( "Edit vertex shader" ),
                this,
                SLOT( ToggleVertexShaderEditor( ) ),
                Qt::Key_V );

    pOption->addAction(
                QIcon( ),
                tr( "Edit fragment shader" ),
                this,
                SLOT( ToggleFragmentShaderEditor( ) ),
                Qt::Key_F );

    pAbout->addAction(
                QIcon( ),
                tr( "About..." ),
                this,
                SLOT( About( ) ),
                Qt::Key_F1 );
    pAbout->addAction(
                QIcon( ),
                tr( "About Qt..."),
                qApp,
                SLOT( aboutQt( ) ),
                Qt::Key_F2 );
    pAbout->addAction(
                QIcon( ),
                tr( "Feedback..."),
                this,
                SLOT( Feedback( ) ),
                Qt::Key_F3 );

    menuBar( )->addMenu( pOption );
    menuBar( )->addMenu( pAbout );
}
/*---------------------------------------------------------------------------*/
void MainWindow::ToggleVertexShaderEditor( void )
{
    ShaderDialog* pDialog = new ShaderDialog( ShaderDialog::_VertexShader_,
                                              g_VertexShaderText,
                                              this );
    connect( pDialog, SIGNAL( CompileAndLinkVertexShader( const QString& ) ),
             (GLWidget*)centralWidget( ), SLOT( CompileAndLinkVertexShader( const QString& ) ) );
    pDialog->show( );
}
/*---------------------------------------------------------------------------*/
void MainWindow::ToggleFragmentShaderEditor( void )
{
    ShaderDialog* pDialog = new ShaderDialog( ShaderDialog::_FragmentShader_,
                                              g_FragmentShaderText,
                                              this );
    connect( pDialog, SIGNAL( CompileAndLinkFragmentShader( const QString& ) ),
             (GLWidget*)centralWidget( ), SLOT( CompileAndLinkFragmentShader( const QString& ) ) );
    pDialog->show( );
}
/*---------------------------------------------------------------------------*/
void MainWindow::About( void )
{
    QString text = tr( "<h1><font color=red>Made by Jiangcaiyang</font></h1>\ncreated in august" );
    QMessageBox::information(
                this,
                tr( "Program information" ),
                text );
}
/*---------------------------------------------------------------------------*/
void MainWindow::Feedback( void )
{
    FeedbackDialog dialog(
                windowTitle( ),
                this );
    dialog.exec( );
}
