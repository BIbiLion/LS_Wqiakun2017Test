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
 * ┃ file name: ShaderKeywordHighlight.cpp                                  ┃
 * ┃ create date: 2013年8月24日星期六 23时28分35秒                          ┃
 * ┃ last modified date: 2013年8月24日星期六 23时28分35秒                   ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include "ShaderKeywordHighlight.h"
/*---------------------------------------------------------------------------*/
ShaderKeywordHighlight::ShaderKeywordHighlight(
        QTextDocument* pParent ):
    QSyntaxHighlighter( pParent )
{
    // 关键词
    m_KeywordFormat.setForeground( Qt::blue );
    foreach ( const QString& pattern,
              QStringList( ) <<
              "void" << "bool" << "int" << "float" << "struct" <<
              "for" << "while" << "do" <<
              "if" << "else" << "break" << "continue" <<
              "bvec2" << "bvec3" << "bvec4" <<
              "ivec2" << "ivec3" << "ivec4" <<
              "vec2" << "vec3" << "vec4" <<
              "mat2" << "mat3" << "mat4" <<
              "mat2x2" << "mat3x3" << "mat4x4" <<
              "mat2x3" << "mat2x4" <<
              "mat3x2" << "mat3x4" <<
              "mat4x2" << "mat4x3" <<
              "sampler1D" << "sampler2D" << "sampler3D" <<
              "samplerCube" <<
              "sampler1DShadow" << "sampler2DShadow" <<
              "discard" )
    {
        m_HighlightRules.append( HighlightRule( ) );
        HighlightRule& rule = m_HighlightRules.last( );

        rule.format     = m_KeywordFormat;
        rule.pattern    = QRegExp( "\\b" + pattern + "\\b" );
    }

    // 限定符
    m_ConstraintFormat.setForeground( Qt::darkMagenta );
    foreach ( const QString& pattern,
              QStringList( ) <<
              "const" << "attribute" << "uniform" << "varying" <<
              "centroid" << "invariant" << "in" << "out" << "inout" )
    {
        m_HighlightRules.append( HighlightRule( ) );
        HighlightRule& rule = m_HighlightRules.last( );

        rule.format     = m_ConstraintFormat;
        rule.pattern    = QRegExp( "\\b" + pattern + "\\b" );
    }

    // OpenGL内置变量
    m_BuiltInVariableFormat.setForeground( Qt::darkRed );
    foreach ( const QString& pattern,
              QStringList( ) <<
              "gl_Color" << "gl_SecondaryColor" <<
              "gl_Normal" << "gl_Vertex" << "gl_MultiTexCoordn" <<
              "gl_FogCoord" << "gl_Position" << "gl_ClipVertex" <<
              "gl_PointSize" << "gl_FrontColor" << "gl_BackColor" <<
              "gl_FrontSecondaryColor" << "gl_BackSecondaryColor" <<
              "gl_TexCoord[]" << "gl_FogFragCoord" << "gl_FragCoord" <<
              "gl_FrontFacing" << "gl_PointCoord" << "gl_FragColor" <<
              "gl_FragData[]" << "gl_FragDepth" )
    {
        m_HighlightRules.append( HighlightRule( ) );
        HighlightRule& rule = m_HighlightRules.last( );

        rule.format     = m_BuiltInVariableFormat;
        rule.pattern    = QRegExp( "\\b" + pattern + "\\b" );
    }

    // 单行注释
    m_CommentFormat.setForeground( Qt::darkGreen );

    m_HighlightRules.append( HighlightRule( ) );
    HighlightRule& rule = m_HighlightRules.last( );

    rule.format         = m_CommentFormat;
    rule.pattern        = QRegExp( "//[^\n]*" );
}
/*---------------------------------------------------------------------------*/
void ShaderKeywordHighlight::highlightBlock( const QString& text )
{
    foreach ( const HighlightRule& rule, m_HighlightRules )
    {
        int index = rule.pattern.indexIn( text );
        while ( index >= 0 )// 找到符合的块
        {
            int length = rule.pattern.matchedLength( );
            setFormat( index, length, rule.format );
            index = rule.pattern.indexIn( text, index + length );
        }
    }
}
/*---------------------------------------------------------------------------*/
