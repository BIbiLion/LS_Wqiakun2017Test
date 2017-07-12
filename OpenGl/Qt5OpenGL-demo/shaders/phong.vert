#version 130
in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 aTexCoord;

in vec3 normal;
out vec3 Normal;//从定点着色器传递值到片段着色器中

uniform mat4 transform;

out vec3 outColor;
out vec2 TexCoord;

out vec3 FragPos;

void main()
{
    outColor = vertexColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);

    //片段位置
    //我们还需要片段的位置。我们会在世界空间中进行所有的光照计算，
    //因此我们需要一个在世界空间中的顶点位置。
    //我们可以通过把顶点位置属性乘以模型矩阵（不是观察和投影矩阵）
    //来把它变换到世界空间坐标。
    FragPos=vec3(transform*vec4(vertexPosition,1.0)) ;

    //关于法线 转换到空间坐标系 没有缩放，可以不用计算 法线矩阵的，计算的话，可以在cpu中计算好，传入到顶点作色期中
   //Normal = mat3(transpose(inverse(transform))) * normal;
    Normal=normal;
    gl_Position =transform* vec4(vertexPosition, 1.0);
}
