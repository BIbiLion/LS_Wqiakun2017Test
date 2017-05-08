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
 * ┃ file name: ShaderDialog.cpp                                            ┃
 * ┃ create date: 2013年8月24日星期六 22时38分40秒                          ┃
 * ┃ last modified date: 2013年8月24日星期六 22时38分40秒                   ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include <QFileDialog>
#include <QFile>
#include "ShaderDialog.h"
#include "ui_ShaderDialog.h"

/*---------------------------------------------------------------------------*/
extern QString g_VertexShaderText;
extern QString g_FragmentShaderText;
/*---------------------------------------------------------------------------*/
ShaderDialog::ShaderDialog( ShaderType type,
                            const QString& shaderText,
                            QWidget *parent ):
    QDialog(parent),
    ui(new Ui::ShaderDialog),
    m_Type( type )
{
    ui->setupUi(this);
    switch ( m_Type )
    {
    case _VertexShader_:
        ui->pHintLabel->setText( tr( "This is a vertex shader" ) ); break;
    case _FragmentShader_:
        ui->pHintLabel->setText( tr( "This is a fragment shader" ) ); break;
    default:;
    }

    ui->pShaderEdit->setPlainText( shaderText );

    setAttribute( Qt::WA_DeleteOnClose );// 需要用槽函数关闭并删除它
}
/*---------------------------------------------------------------------------*/
ShaderDialog::~ShaderDialog()
{
    delete ui;
}
/*---------------------------------------------------------------------------*/
void ShaderDialog::on_pCompileButton_clicked()
{
    // 调用槽函数编译链接着色器并保存着色器内容
    switch ( m_Type )
    {
    case _VertexShader_:
        g_VertexShaderText = ui->pShaderEdit->toPlainText( );
        emit CompileAndLinkVertexShader( g_VertexShaderText ); break;
    case _FragmentShader_:
        g_FragmentShaderText = ui->pShaderEdit->toPlainText( );
        emit CompileAndLinkFragmentShader( g_FragmentShaderText ); break;
    default:;
    }

    // 关闭并删除
    close( );
}
/*---------------------------------------------------------------------------*/

void ShaderDialog::on_pSaveIcon_clicked()
{
    QString extension;

    switch ( m_Type )
    {
    case _VertexShader_:
        extension = ".vert";
        break;
    case _FragmentShader_:
        extension = ".frag";
        break;
    default:;
    }

    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr( "Please select and enter to save" ),
                qApp->applicationDirPath( ),
                tr( "Shader file (*.%1)" ).arg( extension ) );
    QFile file( fileName );
    if ( !file.open( QIODevice::WriteOnly ) ) return;
    file.write( ui->pShaderEdit->toPlainText( ).toUtf8( ) );
    file.close( );
}
