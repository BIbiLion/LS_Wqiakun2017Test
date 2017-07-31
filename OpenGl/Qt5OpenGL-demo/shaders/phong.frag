#version 130

in vec3 outColor;

//纹理
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D outTexture;


//光照
uniform vec3 lightColor;
in vec3 Normal;
in vec3 FragPos;//片段位置
uniform vec3 lightPos; // 反射光源的位置
in vec3 viewPos ;//观察位置

//材质

/*每个物体都拥有自己独特的材质从而对光照做出不同的反应的方法
  ambient材质向量定义了在环境光照下这个物体反射得是什么颜色，通常这是和物体颜色相同的颜色。
  diffuse材质向量定义了在漫反射光照下物体的颜色。（和环境光照一样）漫反射颜色也要设置为我们需要的物体颜色。
  specular材质向量设置的是镜面光照对物体的颜色影响（或者甚至可能反射一个物体特定的镜面高光颜色）。
  最后，shininess影响镜面高光的散射/半径。
  */
struct Material
{
     vec3 ambient;
     vec3 diffuse;
     vec3 sepecuar;
     float shininess;
};
uniform Material material;

//材质和纹理结合  漫反射贴图 镜面贴图
//引入漫反射和镜面光贴图(Map)。这允许我们对物体的漫反射分量（以及间接地对环境光分量，它们几乎总是一样的）和镜面光分量有着更精确的控制。
struct Material_am
{
   sampler2D diffuse;
   sampler2D sepecuar;
   float shininess;
};

void main()
{
/*//光照
    //ambient 环境光

    float ambientStrength =0.2;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result=ambient *outColor;

    //定点颜色和环境光颜色混合
    // FragColor=vec4(result, 1.0);

    //diffuse反射光
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //result=diffuse*outColor;

    result = (ambient + diffuse) * outColor;
    //FragColor=vec4(result, 1.0);


    //special
    float specularStrength = 0.5;
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 reflectDir  = reflect(-lightDir, norm);
    //256高光的反光度一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
    float spec  = pow(max(dot(viewDir, reflectDir), 0.0), 23);
    vec3 specular  = specularStrength *spec * lightColor;
     result  = (ambient + diffuse + specular) * outColor;

     //result=  vec3((result,1.0)*texture2D(outTexture,TexCoord));

     FragColor  = vec4(result, 1.0);
*/

//[材质]
  //设置强度值

    //ambient 环境光
    vec3 ambient = material.ambient * lightColor;


    //定点颜色和环境光颜色混合
    // FragColor=vec4(result, 1.0);

    //diffuse反射光
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff*material.diffuse) * lightColor;

    //special
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 reflectDir  = reflect(-lightDir, norm);
    //256高光的反光度一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
    float spec  = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular  = material.sepecuar *spec * lightColor;


    vec3 result=ambient +diffuse+specular;
     FragColor  = vec4(result, 1.0);

//[材质]

    //纹理颜色和定点颜色混合
    //FragColor=texture2D(outTexture,TexCoord)*vec4(outColor, 1.0);
    //输出定点颜色
    //FragColor=vec4(outColor, 1.0);
}
