
#version 330 core
#extension GL_NV_shadow_samplers_cube: enable//添加扩展 启用
in vec3 TexCoords; //textureDir 代表3D纹理坐标的方向向量
uniform samplerCube  cubemap; // 立方体贴图的纹理采样器
//uniform sampler2D cubemap;
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
  //添加环境光

    float ambientStrength =0.5;
    vec3 ambient = ambientStrength * lightColor;

    //FragColor= textureCube (cubemap,TexCoords)*(ambient,1.0);

    FragColor = vec4(0,0.5f,1.0f,0.5f) ;
}

/*
out vec4 FragColor;
uniform vec3 color;
void main(void)
{
   // vec4 color=vec4(0,0.5f,1.0f,0.5f);
    FragColor = vec4(color,1.0f) ;
}*/
