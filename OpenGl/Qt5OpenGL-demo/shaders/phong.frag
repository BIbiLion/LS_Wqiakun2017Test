#version 130

in vec3 outColor;

in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D outTexture;

uniform vec3 lightColor;

in vec3 Normal;
in vec3 FragPos;//片段位置
uniform vec3 lightPos; // 反射光源的位置


in vec3 viewPos ;//观察位置
void main()
{

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

    //纹理颜色和定点颜色混合
    //FragColor=texture2D(outTexture,TexCoord)*vec4(outColor, 1.0);
    //输出定点颜色
    //FragColor=vec4(outColor, 1.0);
}
