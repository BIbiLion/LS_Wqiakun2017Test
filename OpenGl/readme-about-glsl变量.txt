
http://blog.csdn.net/jackers679/article/details/6848085
最近研究了下OpenGL ES2.0的programing guide。看到shader里面声明的变量一般有三种类型，特地为此做个小的总结。

 
1.uniform变量

uniform变量是外部application程序传递给（vertex和fragment）shader的变量。因此它是application通过函数glUniform**（）函数赋值的。在（vertex和fragment）shader程序内部，uniform变量就像是C语言里面的常量（const ），它不能被shader程序修改。（shader只能用，不能改）

如果uniform变量在vertex和fragment两者之间声明方式完全一样，则它可以在vertex和fragment共享使用。（相当于一个被vertex和fragment shader共享的全局变量）

uniform变量一般用来表示：变换矩阵，材质，光照参数和颜色等信息。

以下是例子：

uniform mat4 viewProjMatrix; //投影+视图矩阵
uniform mat4 viewMatrix;        //视图矩阵
uniform vec3 lightPosition;     //光源位置

 
2.attribute变量

attribute变量是只能在vertex shader中使用的变量。（它不能在fragment shader中声明attribute变量，也不能被fragment shader中使用）

一般用attribute变量来表示一些顶点的数据，如：顶点坐标，法线，纹理坐标，顶点颜色等。

在application中，一般用函数glBindAttribLocation（）来绑定每个attribute变量的位置，然后用函数glVertexAttribPointer（）为每个attribute变量赋值。

以下是例子：

uniform mat4 u_matViewProjection;
attribute vec4 a_position;
attribute vec2 a_texCoord0;
varying vec2 v_texCoord;
void main(void)
{
gl_Position = u_matViewProjection * a_position;
v_texCoord = a_texCoord0;
}
3.varying变量

varying变量是vertex和fragment shader之间做数据传递用的。一般vertex shader修改varying变量的值，然后fragment shader使用该varying变量的值。因此varying变量在vertex和fragment shader二者之间的声明必须是一致的。application不能使用此变量。

以下是例子：

// Vertex shader
uniform mat4 u_matViewProjection;
attribute vec4 a_position;
attribute vec2 a_texCoord0;
varying vec2 v_texCoord; // Varying in vertex shader
void main(void)
{
gl_Position = u_matViewProjection * a_position;
v_texCoord = a_texCoord0;
}


// Fragment shader
precision mediump float;
varying vec2 v_texCoord; // Varying in fragment shader
uniform sampler2D s_baseMap;
uniform sampler2D s_lightMap;
void main()
{
vec4 baseColor;
vec4 lightColor;
baseColor = texture2D(s_baseMap, v_texCoord);
lightColor = texture2D(s_lightMap, v_texCoord);
gl_FragColor = baseColor * (lightColor + 0.25);
}

 

 