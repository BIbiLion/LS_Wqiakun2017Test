/*
   //gl_FragCoord的x和y元素是当前片段的窗口空间坐标（window-space coordinate）。
   它们的起始处是窗口的左下角。如果我们的窗口是800×600的，
   那么一个片段的窗口空间坐标x的范围就在0到800之间，y在0到600之间。

   //gl_FrontFacing变量是一个布尔值，如果当前片段是正面的一部分那么就是true，
   否则就是false。这样我们可以创建一个立方体，里面和外面使用不同的纹理：
*/
#version 330 core
out vec4 FragColor;
uniform vec3 color;
void main(void)
{
   // vec4 color=vec4(0,0.5f,1.0f,0.5f);
    FragColor = vec4(color,1.0f) ;

}
